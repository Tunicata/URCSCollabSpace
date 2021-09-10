def is_prime(n):
    if n % 5 == 0 or n % 7 == 0:
        return False
    else:
        return True


def prime_generator(n):
    if n > 7:
        num = list(range(5, n + 1, 6)) + list(range(7, n + 1, 6))
        primes = [x for x in num if is_prime(x)] + [2, 3, 5, 7]
        primes.sort()
    elif n == 7:
        primes = [2, 3, 5, 7]
    elif 5 <= n <= 6:
        primes = [2, 3, 5]
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
    print(output)
    return output


if __name__ == '__main__':
    prime_partition(32)