import math


def is_prime(n):
    sample = math.ceil(math.sqrt(n))
    while sample > 1:
        if n % sample == 0:
            return False
        sample -= 1
    return True


def prime_generator(n):
    if n > 4:
        num = list(range(5, n + 1, 6)) + list(range(7, n + 1, 6))
        primes = [x for x in num if is_prime(x)] + [2, 3]
        primes.sort()
    elif 3 <= n <= 4:
        primes = [2, 3]
    elif n == 2:
        primes = [2]
    else:
        primes = []
    return primes


def prime_partition_ite(num, ptr, lst, sample, output):
    if ptr < 0:
        half = num >> 1
        for i in range(ptr + 1, len(sample)):
            if sample[i] > half:
                break
            lst.append(sample[i])
            prime_partition_ite(num, i, lst, sample, output)
            lst.pop()
    else:
        rest = num - sample[ptr]
        if rest == 0:
            output.append(lst.copy())
        elif rest > 0:
            for i in range(ptr + 1, len(sample)):
                if sample[i] > num:
                    break
                lst.append(sample[i])
                prime_partition_ite(rest, i, lst, sample, output)
                lst.pop()


def prime_partition(num):
    output = []
    prime_partition_ite(num, -1, [], prime_generator(num), output)
    return output


if __name__ == '__main__':
    print("Please enter an Integer Larger than 1 to find its prime partitions: ", end="")
    try:
        n = int(input())
        partitions = prime_partition(n)
        for partition in partitions:
            print(partition)
    except ValueError:
        print("Input not an integer")
