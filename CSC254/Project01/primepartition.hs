findPrime [] = [] 
findPrime (x:xs) = x : findPrime (filter (\a -> not $ a `mod` x == 0) xs)

subsets :: [Int] -> [[Int]]
subsets []  = [[]]
subsets (x:xs) = subsets xs ++ map (x:) (subsets xs)

main = do
    putStrLn "Enter the number: "
    input <- getLine
    let x = (read input :: Int)
    print (filter ((==x) . sum) $ subsets(findPrime[2..x]))
