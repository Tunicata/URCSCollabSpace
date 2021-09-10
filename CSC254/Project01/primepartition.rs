fn is_prime(num : &i32) -> bool{
    if num % 5 == 0 || num % 7 == 0{
        return false;
    } else {
        return true;
    }
}

fn prime_generator(num : i32) -> Vec<i32>{
    if num > 7{
        let mut _curr_fi:i32 = 5;
        let mut _curr_se:i32 = 7;
        let mut range_lst:Vec<i32> = vec![2, 3, 5, 7];


        loop {
            _curr_fi += 6;
            if _curr_fi > num{
                break;
            }
            if is_prime(&_curr_fi){
                range_lst.push(_curr_fi.clone());
            }
            _curr_se += 6;
            if _curr_se > num{
                break;
            }
            if is_prime(&_curr_se){
                range_lst.push(_curr_se.clone());
            }
        }

        return range_lst;
    } else if num == 7 {
        return vec![2, 3, 5, 7];
    } else if num >= 5 && num <= 6 {
        return vec![2, 3, 5];
    } else if num >= 3 && num <= 4{
        return vec![2, 3];
    } else if num == 2{
        return vec![2];
    } else {
        return vec![];
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
