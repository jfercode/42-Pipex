#!/bin/bash

# Variables globales
TESTS=0
PASSED=0
FAILED_LOG="failed_tests.log"

# Función para imprimir encabezados
print_header() {
    echo "============================"
    echo "Running tests for pipex..."
    echo "============================"
}

# Función para verificar si el ejecutable 'pipex' está disponible
check_executable() {
    if ! [ -x "./pipex" ]; then
        echo "Error: 'pipex' executable not found or not executable."
        exit 1
    fi
}

# Función para ejecutar un caso de prueba
run_test() {
    TESTS=$((TESTS + 1))
    local description="$1"
    local cmd="$2"
    local expected_file="$3"
    local output_file="$4"
    local expect_failure="$5"

    echo -e "\nTest $TESTS: $description"
    echo "Command: $cmd"

    # Ejecuta el programa y guarda la salida
    output=$(eval "$cmd" 2>&1)
    exit_code=$?

    # Comprobamos si se espera fallo
    if [[ $expect_failure -eq 1 ]]; then
        if [[ $exit_code -ne 0 ]]; then
            echo "✅ Expected failure occurred (non-zero exit code)"
            PASSED=$((PASSED + 1))
        else
            echo "❌ Expected failure but command succeeded"
            echo "Expected failure, but command returned exit code 0:"
            echo "$output"
            echo "-----------------------------------" >> "$FAILED_LOG"
            echo "Test $TESTS failed: $description" >> "$FAILED_LOG"
            echo "$output" >> "$FAILED_LOG"
            echo "-----------------------------------" >> "$FAILED_LOG"
        fi
    else
        # Compara salidas solo si no se espera un fallo
        if diff -u "$expected_file" "$output_file" &> /dev/null; then
            echo "✅ Functional test PASSED"
            PASSED=$((PASSED + 1))
        else
            echo "❌ Functional test FAILED"
            echo "Expected:"
            cat "$expected_file"
            echo "Got:"
            echo "$output"

            # Registrar fallo en el archivo de log
            echo "Test $TESTS failed: $description" >> "$FAILED_LOG"
            echo "Expected output:" >> "$FAILED_LOG"
            cat "$expected_file" >> "$FAILED_LOG"
            echo "Actual output:" >> "$FAILED_LOG"
            echo "$output" >> "$FAILED_LOG"
            echo "-----------------------------------" >> "$FAILED_LOG"
        fi
    fi
}

# Función para generar resultados esperados
generate_expected() {
    echo "Generating expected results..."
    mkdir -p inputs outputs expected
    echo "line1" > inputs/infile
    echo "line2" >> inputs/infile
    cat inputs/infile > expected/out1
    cat inputs/infile | wc -l > expected/out2
    echo "line2" > expected/out3
    touch expected/empty_out
    echo "filecontent" > inputs/specialfile
    echo "empty" > expected/empty_cmd
    touch inputs/emptyfile  # Asegurarse de que el archivo vacío exista
}

# Función principal
main() {
    # Verificar si el ejecutable 'pipex' existe antes de continuar
    check_executable

    # Crear los directorios necesarios solo si 'pipex' está presente
    generate_expected

    print_header

    # Eliminar el archivo de fallos si existe de ejecuciones anteriores
    rm -f "$FAILED_LOG"

    # Funciones de prueba
    run_test "Basic: cat infile > outfile" \
        "./pipex inputs/infile 'cat' 'cat' outputs/out1" \
        "expected/out1" "outputs/out1" 0

    run_test "Word count: cat infile | wc -l > outfile" \
        "./pipex inputs/infile 'cat' 'wc -l' outputs/out2" \
        "expected/out2" "outputs/out2" 0

    run_test "Filter and save: grep 'line2' > outfile" \
        "./pipex inputs/infile 'grep line2' 'cat' outputs/out3" \
        "expected/out3" "outputs/out3" 0

    # Prueba de comandos vacios
    run_test "Empty command 1" \
        "./pipex inputs/infile '' 'cat' outputs/empty_cmd" \
        "/dev/null" "outputs/empty_cmd" 1
    
    run_test "Empty command 2" \
        "./pipex inputs/infile 'cat' '' outputs/empty_cmd" \
        "/dev/null" "outputs/empty_cmd" 1

    # Manejo de errores
    run_test "Non-existent input file" \
        "./pipex inputs/nonexistent 'cat' 'cat' outputs/out4" \
        "/dev/null" "outputs/out4" 1

    run_test "Invalid command" \
        "./pipex inputs/infile 'invalidcmd' 'cat' outputs/out5" \
        "/dev/null" "outputs/out5" 1

    # Limpieza de directorios de salida, entrada y esperado
    rm -rf outputs
    rm -rf inputs
    rm -rf expected
    chmod 644 inputs/specialfile 2> /dev/null

    # Resumen
    echo -e "\n============================"
    echo "Tests completed: $TESTS"
    echo "Tests passed:    $PASSED"
    echo "Tests failed:    $((TESTS - PASSED))"
    echo "============================"

    # Mostrar mensaje sobre el archivo de fallos si hay errores
    if [ $((TESTS - PASSED)) -gt 0 ]; then
        echo "Some tests failed. See the detailed report in '$FAILED_LOG'."
    else
        echo "All tests passed successfully!"
    fi
}

main
