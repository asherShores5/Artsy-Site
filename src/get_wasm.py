# Make sure to use "pip install wasmer wasmer_compiler_cranelift" first
import wasmer
from wasmer_compiler_cranelift import Compiler
import sys

if len(sys.argv) > 1:
    # Load the WebAssembly Text string
    wat_text = sys.argv[1]

    # Convert WebAssembly Text to WebAssembly Binary
    wasm_bytes = wasmer.wat2wasm(wat_text)

    # Create a store
    store = wasmer.Store(wasmer.engine.Universal(Compiler))

    # Compile the WebAssembly binary to a module
    module = wasmer.Module(store, wasm_bytes)

    # Define import functions
    def print_int_func(x):
        print(x)
    
    def print_float_func(x):
        print(x)
    
    def print_string_func(x):
        print(x.decode('utf-8'))
    
    def newline_func():
        print()

    printInt = wasmer.Function(store, print_int_func)
    printFloat = wasmer.Function(store, print_float_func)
    printString = wasmer.Function(store, print_string_func)
    newline = wasmer.Function(store, newline_func)


    # Define the import object with the required functions
    import_object = wasmer.ImportObject()
    import_object.register(
        "env",
        {
            'print_int': printInt,
            'print_float': printFloat,
            'print_string': printString,
            'newline': newline
        }
    )

    # Instantiate the WebAssembly module
    instance = wasmer.Instance(module, import_object)

    # Get the exports object
    exports = instance.exports

else:
    print("No string argument provided.")
