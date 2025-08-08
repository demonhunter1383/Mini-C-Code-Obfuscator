from cli import parse_args  
from lexer_parser.parser import parser
from obfuscation_passes import RenameVariables, RemoveDeadCode, InsertOpaquePredicates
from codegen import CodeGenerator
import time
import os
from ast_visualizer import ASTVisualizer


def apply_obfuscation_passes(ast, passes):
    for obfuscation_pass in passes:
        obfuscation_pass.apply(ast)


def main():
    args = parse_args() 

    # Read input C code
    with open(args.input, 'r') as f:
        source_code = f.read()

    original_size = len(source_code.encode('utf-8'))

    # Parse the code into AST
    ast = parser.parse(source_code)

    # Collect passes to apply
    passes = []
    if args.rename_vars:
        passes.append(RenameVariables())
    if args.remove_dead_code:
        passes.append(RemoveDeadCode())
    if args.insert_opaque:
        passes.append(InsertOpaquePredicates())

    # Apply obfuscation (with timing)
    start_time = time.time()
    apply_obfuscation_passes(ast, passes)
    end_time = time.time()

    # Generate obfuscated code
    generator = CodeGenerator()
    obfuscated_code = generator.generate(ast)

    # Write to output file
    with open(args.output, 'w') as f:
        f.write(obfuscated_code)

    obfuscated_size = len(obfuscated_code.encode('utf-8'))
    # Optional: visualize the AST before or after obfuscation
    visualizer = ASTVisualizer()
    visualizer.render(ast, 'examples/ast_visualized.png')

    # Print results
    print("Obfuscation completed. Output written to:", args.output)
    print(f"Obfuscation time: {end_time - start_time:.4f} seconds")
    print(f"Original size: {original_size} bytes")
    print(f"Obfuscated size: {obfuscated_size} bytes")
    print(f"Size change: {obfuscated_size - original_size:+} bytes")


if __name__ == "__main__":
    main()
