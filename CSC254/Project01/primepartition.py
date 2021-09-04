def prime_generator(num):
    if num < 2:
        return []
    elif num == 2:
        return [2]
    else:
        excludes = set()
        primes = [2]
        mark = 3
        while mark < num:
            if excludes and mark in excludes:
                excludes.remove(mark)
            else:
                primes.append(mark)
            for i in range(1, len(primes)):
                exclude = mark * primes[i]
                if exclude > num:
                    break
                excludes.add(exclude)
            mark += 2
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