use std::collections::BTreeSet;


fn prime_generator(num : i32) -> Vec<i32>{
	if num < 2{
        return Vec::new();
    } else if num == 2{
        return vec![2];
    } else {
        let mut _excludes:BTreeSet<i32> = BTreeSet::new();
        let mut primes:Vec<i32> = vec![2];
        let mut mark:i32 = 3;
        while mark < num{
            if !_excludes.is_empty() && _excludes.contains(&mark){
                _excludes.remove(&mark);
            } else {
                primes.push(mark);
            }
            for i in 1..primes.len() {
                let exclude = mark * primes[i];
                if exclude > num{
                    break;
                }
                _excludes.insert(exclude);
            }
            mark += 2;
        }
        return primes;
    }
}


fn prime_partition_ite(num : i32, ptr : usize, lst : &mut Vec<i32>, sample : &Vec<i32>, output : &mut Vec<Vec<i32>>, start_flag : bool){
    if !start_flag {
        let half = num >> 1;
        for i in ptr..sample.len(){
            if sample[i] > half{
                break;
            }
            lst.push(sample[i]);
            prime_partition_ite(num, i, lst, sample, output, true);
            lst.pop();
        }
    } else {
            let rest = num - sample[ptr];
            if rest == 0{
                let mut solution:Vec<i32> = Vec::new();
                for prime in lst{
                    solution.push(prime.clone());
                }
                output.push(solution);
            } else if rest > 0 {
                for i in (ptr + 1)..sample.len(){
                    if sample[i] > num{
                        break;
                    }
                    lst.push(sample[i]);
                    prime_partition_ite(rest, i, lst, sample, output, true);
                    lst.pop();
                }
            }
    }
}


fn prime_partition(num : i32) -> Vec<Vec<i32>>{
    let mut output:Vec<Vec<i32>> = Vec::new();
    prime_partition_ite(num, 0, &mut Vec::new(), &prime_generator(num), &mut output, false);
    println!("{:?}", output);
    return output;
}


fn main() {
    prime_partition(32);
}
