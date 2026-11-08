#!/bin/bash

APP="./build/app"

if [ ! -f "$APP" ]; then
    echo "错误: 找不到可执行文件 $APP，请先编译！"
    exit 1
fi

run_test() {
    local expr="$1"
    local expected="$2"
    
    # 加了 timeout 2s，防止你的 lexer 遇到奇怪字符死循环卡住脚本
    local output
    output=$(printf "%s\nq\n" "$expr" | timeout 2s "$APP")
    
    # 检查是不是超时被杀掉了
    if [ $? -eq 124 ]; then
        echo -e "❌ FAIL | \033[31m表达式: $expr\033[0m | 期望: $expected, 实际: 死循环超时！"
        return
    fi

    local actual
    actual=$(echo "$output" | grep -oP 'Result: \K[0-9.-]+' | head -n 1)

    if [ "$actual" == "$expected" ]; then
        echo -e "✅ PASS | \033[32m表达式: $expr\033[0m | 结果: $actual"
    else
        echo -e "❌ FAIL | \033[31m表达式: $expr\033[0m | 期望: $expected, 实际: $actual"
    fi
}

echo "==================================="
echo "🚀 开始运行纯整数/分数测试集"
echo "==================================="

# 1. 基础优先级测试
run_test "1 + 2 * 3" "7.00"
run_test "3 * 4 + 2" "14.00"
run_test "(1 + 2) * 3" "9.00"

# 2. 浮点/分数运算测试
run_test "1 / 2" "0.50"
run_test "5 / 2 + 1" "3.50"
run_test "10 / 4" "2.50"

# 3. 连续运算（左结合终极测试：极易翻车！）
run_test "8 - 4 - 2" "2.00"
run_test "100 / 10 / 2" "5.00"
run_test "20 - 5 * 2 + 10 / 2" "15.00"

# 4. 嵌套括号测试
run_test "((1 + 2) * (3 + 4))" "21.00"
run_test "1 * (2 + 3 * (4 - 1))" "11.00"

# 5. 异常测试
echo "-----------------------------------"
echo "🧨 测试异常输入"
echo "-----------------------------------"

echo -n "测试除零 (1 / 0): "
printf "1 / 0\nq\n" | timeout 2s "$APP" | grep -q "Invalid Input" && echo "✅ PASS" || echo "❌ FAIL"

echo -n "测试括号缺失 (1 + (2 * 3): "
printf "1 + (2 * 3\nq\n" | timeout 2s "$APP" | grep -q "Invalid Input" && echo "✅ PASS" || echo "❌ FAIL (是否崩溃？)"

echo -n "测试非法字符 (1 + a): "
printf "1 + a\nq\n" | timeout 2s "$APP" | grep -q "Invalid Input" && echo "✅ PASS" || echo "❌ FAIL (是否死循环？)"
