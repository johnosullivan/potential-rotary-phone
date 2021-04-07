source "./assert.sh"

## Super basic test for additive ops.
test1 () {
    echo "Test #1 = 8"
    local expected actual
    expected="8"
    actual=$(../mylang test1)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test1

test2 () {
    echo "Test #2 = 5"
    local expected actual
    expected="5"
    actual=$(../mylang test2)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test2

test3 () {
    echo "Test #3 = 65"
    local expected actual
    expected="65"
    actual=$(../mylang test3)
    assert_eq "$expected" "$actual" "not equivalent!"
}

test3