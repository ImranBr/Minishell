#!/bin/bash

MINISHELL=./minishell
LOG="minishell_test.log"
REF_BASH="bash_ref.tmp"
YOUR_OUT="your_output.tmp"

echo "===== DÉBUT DES TESTS =====" > "$LOG"

# Liste de tests simples à automatiser (sélection représentative)
tests=(
    "echo Hola"
    "echo -n Hola"
    "echo \"\""
    ">"
    ">>"
    "echo \$?"
    "echo \$HOME"
    "echo \$HOME9"
    "echo \$hola"
    "export HOLA=bonjour && env | grep HOLA"
    "unset HOLA && env | grep HOLA"
    "pwd"
    "pwd hola"
    "cd .. && pwd"
    "exit 42"
)

i=1
for cmd in "${tests[@]}"
do
    echo -e "\nTest $i: $cmd" | tee -a "$LOG"
    
    echo "$cmd" | bash --posix > "$REF_BASH" 2>&1
    echo "$cmd" | $MINISHELL > "$YOUR_OUT" 2>&1

    if diff -u "$REF_BASH" "$YOUR_OUT" > /dev/null; then
        echo "✅ OK" | tee -a "$LOG"
    else
        echo "❌ KO" | tee -a "$LOG"
        echo "--- Diff ---" >> "$LOG"
        diff -u "$REF_BASH" "$YOUR_OUT" >> "$LOG"
    fi
    ((i++))
done

rm -f "$REF_BASH" "$YOUR_OUT"
echo -e "\n===== FIN DES TESTS =====" >> "$LOG"
