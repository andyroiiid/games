function hanoi(a, b, c, n)
    if n <= 1 then
        print(a .. " -> " .. c)
    else
        hanoi(a, c, b, n - 1)
        print(a .. " -> " .. c)
        hanoi(b, a, c, n - 1)
    end
end

hanoi("A", "B", "C", 3)
