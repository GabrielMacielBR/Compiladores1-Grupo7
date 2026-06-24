#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
TEST_DIR="$ROOT_DIR/tests"

cd "$SRC_DIR"
make || { echo "Build failed"; exit 1; }

mkdir -p "$TEST_DIR/out"

fail_count=0

run_test() {
  local file="$1"
  local out="$TEST_DIR/out/$(basename "$file").out"
  echo "Running $file"
  ./parser "$file" > "$out" 2>&1 || true

  # Basic sanity checks: AST printed and Tabela de Símbolos printed
  if grep -q "AST:" "$out" && grep -q "Tabela de Símbolos" "$out"; then
    echo "PASS: $(basename "$file")"
  else
    echo "FAIL: $(basename "$file") - missing AST or table"
    fail_count=$((fail_count+1))
  fi
}

echo "Running test_func_mismatch.c (expect semantic error)"
MANIFEST="$TEST_DIR/expected_results.txt"

if [ ! -f "$MANIFEST" ]; then
  echo "Manifest $MANIFEST not found"; exit 1
fi

while read -r line; do
  # skip comments and empty lines
  line="${line%%#*}"
  line="$(printf '%s' "$line" | tr -d '\r' | xargs)"
  [ -z "$line" ] && continue
  file_rel=$(echo "$line" | awk '{print $1}')
  expect=$(echo "$line" | awk '{print $2}')
  file="$ROOT_DIR/$file_rel"
  out="$TEST_DIR/out/$(basename "$file_rel").out"

  echo "Running $file_rel (expect: $expect)"
  "$SRC_DIR"/parser "$file" > "$out" 2>&1 || true

  case "$expect" in
    ok)
      if ! grep -q "AST:" "$out" || ! grep -q "Tabela de Símbolos" "$out" || grep -q "Erro semântico" "$out" || grep -q "Erro sintático" "$out"; then
        echo "FAIL: $(basename "$file") - expected OK but got warnings/errors"
        fail_count=$((fail_count+1))
      else
        # extract AST block (from 'AST:' to the next blank line) and compare to fixture if exists
  AST_OUT="$TEST_DIR/out/$(basename "$file_rel").ast"
           # concatenate all AST blocks (everything between 'AST:' and the next 'Tabela de Símbolos')
           awk 'BEGIN{flag=0} /^AST:/{flag=1; next} /^Tabela de Símbolos/{flag=0} flag && NF{print}' "$out" > "$AST_OUT" || true
        FIX="$TEST_DIR/expected_ast/$(basename "$file_rel").ast"
        if [ -f "$FIX" ]; then
          # normalize: collapse whitespace and remove blank lines for robust comparison
             tr -s '[:space:]' ' ' < "$FIX" | sed 's/^ //; s/ $//' | sed 's/;[[:space:]]*/;/g' > "$FIX.norm"
             tr -s '[:space:]' ' ' < "$AST_OUT" | sed 's/^ //; s/ $//' | sed 's/;[[:space:]]*/;/g' > "$AST_OUT.norm"
          if diff -u "$FIX.norm" "$AST_OUT.norm" > /dev/null; then
            echo "AST MATCH for $(basename "$file_rel")"
          else
            echo "FAIL: AST mismatch for $(basename "$file_rel")"
            echo "--- expected ---"; sed -n '1,200p' "$FIX"; echo "--- got ---"; sed -n '1,200p' "$AST_OUT"
            fail_count=$((fail_count+1))
          fi
          rm -f "$FIX.norm" "$AST_OUT.norm"
        else
          echo "No AST fixture for $(basename "$file_rel"), skipping AST structural check"
        fi

        # extract Tabela block (from 'Tabela de Símbolos' to next blank line)
  TAB_OUT="$TEST_DIR/out/$(basename "$file_rel").table"
           # extract the last 'Tabela de Símbolos' block robustly (handles EOF without trailing blank line)
          awk '
            /^Tabela de Símbolos/{p=1; next}
            p {
              # stop if we hit an INFO/TAC header or an empty line
              if (/^INFO:/ || /^--- TAC emitted/ || /^$/) {
                if (buf!="") { last=buf; buf=""; p=0 }
              } else {
                buf = buf $0 "\n"
              }
            }
            END { if (buf!="") last=buf; if (last!="") print last }
          ' "$out" > "$TAB_OUT" || true
        FIXT="$TEST_DIR/expected_table/$(basename "$file_rel").table"
        if [ -f "$FIXT" ]; then
          # normalize table: remove '(hash)' and collapse whitespace
          # For tables, preserve one entry per line: remove '(hash)', trim spaces, drop empty lines, then sort
          sed 's/(hash)//g' "$FIXT" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//' | grep -v '^$' | sort > "$FIXT.norm"
          sed 's/(hash)//g' "$TAB_OUT" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//' | grep -v '^$' | sort > "$TAB_OUT.norm"
          if diff -u "$FIXT.norm" "$TAB_OUT.norm" > /dev/null; then
            echo "Table MATCH for $(basename "$file_rel")"
          else
            echo "FAIL: Table mismatch for $(basename "$file_rel")"
            echo "--- expected ---"; sed -n '1,200p' "$FIXT"; echo "--- got ---"; sed -n '1,200p' "$TAB_OUT"
            fail_count=$((fail_count+1))
          fi
          rm -f "$FIXT.norm" "$TAB_OUT.norm"
        else
          echo "No Table fixture for $(basename "$file_rel"), skipping table structural check"
        fi
      fi
      ;;
    semantic_error)
      if grep -q "Erro semântico" "$out"; then
        echo "PASS: $(basename "$file") (semantic error detected)"
      else
        echo "FAIL: $(basename "$file") (missing semantic error)"
        fail_count=$((fail_count+1))
      fi
      ;;
    syntax_error)
      if grep -q "Erro sintático" "$out" || grep -q "[ERRO SINTATICO]" "$out"; then
        echo "PASS: $(basename "$file") (syntax error detected)"
      else
        echo "FAIL: $(basename "$file") (missing syntax error)"
        fail_count=$((fail_count+1))
      fi
      ;;
    *)
      echo "Unknown expectation '$expect' for $file";
      fail_count=$((fail_count+1));;
  esac

done < "$MANIFEST"

if [ "$fail_count" -eq 0 ]; then
  echo "All tests passed"
else
  echo "$fail_count tests failed"
  exit 1
fi
