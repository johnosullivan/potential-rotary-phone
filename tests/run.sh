source "./tests/assert.sh"

test1 () {
    echo "Test #1 simple print - 1"
    local expected actual
    expected="1"
    actual=$(./facile -f ./tests/ptests/test1.fx)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test1

test2 () {
    echo "Test #2 simple print - facile"
    local expected actual
    expected="facile"
    actual=$(./facile -f ./tests/ptests/test2.fx)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test2

test3 () {
    echo "Test #3 vars x,y,z print = 30"
    local expected actual
    expected="30"
    actual=$(./facile -f ./tests/ptests/test3.fx)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test3