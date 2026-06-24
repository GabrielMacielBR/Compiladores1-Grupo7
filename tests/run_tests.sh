#!/bin/bash

INPUTS_DIR="./inputs"
EXPECTED_DIR="./expected_outputs"

PASS=0
FAIL=0

normalize() {
    sed 's/[[:space:]]*$//' "$1" | grep -v '^[[:space:]]*$'
}

echo "=============================="
echo " transpilador C -> Python"
echo "=============================="
echo ""

echo ">> rodando make..."
make -C ../src
if [ $? -ne 0 ]; then
    echo "ERRO: make falhou. abortando testes."
    exit 1
fi
echo ""

for input in "$INPUTS_DIR"/*.c; do
    name=$(basename "$input" .c)
    expected="$EXPECTED_DIR/$name.py"

    if [ ! -f "$expected" ]; then
        echo "[SKIP] $name — arquivo esperado não encontrado: $expected"
        continue
    fi

    ../src/parser "$input" > /dev/null 2>&1

    if [ ! -f "result.py" ]; then
        echo "[FAIL] $name — result.py não foi gerado"
        FAIL=$((FAIL + 1))
        continue
    fi

    got=$(normalize result.py)
    exp=$(normalize "$expected")

    if [ "$got" = "$exp" ]; then
        echo "[PASS] $name"
        PASS=$((PASS + 1))
    else
        echo "[FAIL] $name"
        echo "  --- esperado ---"
        normalize "$expected" | sed 's/^/  /'
        echo "  --- obtido ---"
        normalize result.py | sed 's/^/  /'
        FAIL=$((FAIL + 1))
    fi

    rm -f result.py
done

echo ""
echo "=============================="
echo " resultado: $PASS passou, $FAIL falhou"
echo "=============================="
echo ""

echo ">> rodando make clean..."
make -C ../src clean