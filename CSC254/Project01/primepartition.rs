use std::io;
fn is_prime(n: &i32) -> bool {
    let num = n.clone();
    let sample = (num as f64).sqrt() as i32;

    for i in 2..=sample {
        if n % i == 0 {
            return false;
        }
    }

    return true;
}

fn prime_generator(num : i32) -> Vec<i32>{
    if num > 4{
        let mut _curr_fi:i32 = 5;
        let mut _curr_se:i32 = 7;
        let mut range_lst:Vec<i32> = vec![2, 3];


        loop {
            if _curr_fi > num{
                break;
            }
            if is_prime(&_curr_fi){
                range_lst.push(_curr_fi.clone());
            }
            _curr_fi += 6;
            if _curr_se > num{
                break;
            }
            if is_prime(&_curr_se){
                range_lst.push(_curr_se.clone());
            }
            _curr_se += 6;
        }

        return range_lst;
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
    return output;
}

fn main() {
    println! ("Please enter an Integer Larger than 1 to find its prime partitions: ");
    let mut input_line = String::new();
    io::stdin()
        .read_line(&mut input_line)
        .expect("Failed to read line");
    let n: i32 = input_line.trim().parse().expect("Input not an integer");
    let partitions : Vec<Vec<i32>> = prime_partition(n);
    for partition in partitions.iter(){
        println! ("{:?}", partition);
    }
}
