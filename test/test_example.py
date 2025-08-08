# test/test_example.py
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from lexer_parser.parser import parser
from obfuscation_passes.rename_variables import RenameVariables
from codegen import CodeGenerator
from obfuscation_passes.dead_code_insertion import RemoveDeadCode
from obfuscation_passes.opaque_predicates import InsertOpaquePredicates

from unittest.mock import patch


def test_insert_opaque_predicates():
    code = """
    int main() {
        int x;
        x = 1;
        return x;
    }
    """
    ast = parser.parse(code)

    # Force the opaque predicate to always be inserted
    with patch("random.random", return_value=0.0):
        InsertOpaquePredicates().apply(ast)

    output = CodeGenerator().generate(ast)

    assert "((1 * 1) == 1)" in output


def test_combined_passes():
    code = """
    int main() {
        int a;
        int b;
        a = 2;
        b = a + 2;
        return b;
    }
    """
    ast = parser.parse(code)
    RenameVariables().apply(ast)
    RemoveDeadCode().apply(ast)
    InsertOpaquePredicates().apply(ast)
    output = CodeGenerator().generate(ast)

    # Confirm renaming, opaque predicate inserted, and dead vars likely inserted
    import re
    assert not re.search(r'\ba\b', output)
    assert not re.search(r'\bb\b', output)
    assert "((1 * 1) == 1)" in output  # opaque predicate
    assert any(name in output for name in ["useless", "dummy", "junk", "tmp", "waste"])


def test_return_preserved():
    code = """
    int main() {
        int a;
        a = 42;
        return a;
    }
    """
    ast = parser.parse(code)
    RenameVariables().apply(ast)
    output = CodeGenerator().generate(ast)

    # Return statement must still exist
    assert "return" in output


def test_dead_code_insertion():
    code = """
    int main() {
        int a;
        int b;
        a = 1;
        b = a + 2;
        return b;
    }
    """
    ast = parser.parse(code)
    RemoveDeadCode().apply(ast)  # despite name, this inserts dead code
    output = CodeGenerator().generate(ast)

    # Expect at least one known dead var inserted
    inserted_names = ["useless", "dummy", "junk", "tmp", "waste"]
    assert any(name in output for name in inserted_names), "Expected a dead variable to be inserted"


def test_rename_variables():
    code = """
    int main() {
        int x;
        int y;
        x = 5;
        y = x + 3;
        return y;
    }
    """
    ast = parser.parse(code)
    RenameVariables().apply(ast)
    output = CodeGenerator().generate(ast)

    # Test that original variable names are not in output
    assert "x" not in output
    assert "y" not in output
