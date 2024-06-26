# 🚀 Minishell Project

The Minishell project is a simplified version of a Unix shell, and it involves several stages of processing the command line input. Here's a detailed explanation of the different parts of the project: the <u>lexer</u>, the <u>expander</u>, the <u>parser</u>, and the <u>executor</u>.

## 1. Lexer (Lexical Analysis) 🔍
The lexer, or lexical analyzer, is the first stage in processing the input string. It converts the raw input string into a series of tokens. Tokens are the smallest units of meaning, such as keywords, identifiers, operators, and delimiters.

- **Input**: Raw command line string (e.g., `echo $USER | grep admin`)
- **Output**: A list of tokens (e.g., `['echo', '$USER', '|', 'grep', 'admin']`)

**Responsibilities of the Lexer:**
- Identifying different types of tokens (e.g., commands, arguments, operators).
- Handling different types of quoting (single quotes, double quotes).
- Recognizing special characters (e.g., pipes `|`, redirections `>`, `<`).

## 2. Expander 🔄
The expander processes the tokens generated by the lexer, expanding variables and performing any necessary substitutions.

- **Input**: List of tokens (e.g., `['echo', '$USER', '|', 'grep', 'admin']`)
- **Output**: List of expanded tokens (e.g., `['echo', 'john', '|', 'grep', 'admin']`)

**Responsibilities of the Expander:**
- Expanding environment variables (e.g., `$USER` to `john`).
- Handling tilde expansion (e.g., `~` to `/home/username`).
- Processing command substitution (e.g., `` `date` ``).

## 3. Parser (Syntax Analysis) 📚
The parser takes the list of tokens and builds a structured representation of the command, often in the form of an abstract syntax tree (AST). This structure reflects the grammatical relationships between the tokens.

- **Input**: List of expanded tokens (e.g., `['echo', 'john', '|', 'grep', 'admin']`)
- **Output**: Abstract Syntax Tree (AST) or similar structured representation.

**Responsibilities of the Parser:**
- Validating the syntax of the command.
- Constructing a hierarchical representation of the command sequence.
- Detecting syntax errors and providing useful error messages.

## 4. Executor 🏃‍♂️
The executor takes the structured representation from the parser and carries out the commands. This involves creating processes, managing input/output redirections, and handling built-in commands.

- **Input**: Abstract Syntax Tree (AST) or structured representation.
- **Output**: Execution of commands and output generation.

**Responsibilities of the Executor:**
- Creating and managing child processes for each command.
- Setting up pipes and redirections.
- Executing built-in commands within the shell process.
- Handling errors during execution and providing feedback.

## Example Workflow 🛠️
Consider the command: `echo $USER | grep admin`

1. **Lexing**: The lexer splits the command into tokens:
   ```plaintext
   ['echo', '$USER', '|', 'grep', 'admin']

2. **Expanding**: The expander processes the tokens, replacing $USER with the actual username (e.g., john):
   ```plaintext
    ['echo', 'john', '|', 'grep', 'admin']

3. **Parsing**: The parser creates a structured representation, such as an AST:
Command:
   ```plaintext
    - echo john
    - pipe
    - grep admin

4. **Executing**: The executor runs the commands:

- Executes echo john, sending output to the pipe.
- Executes grep admin, reading input from the pipe and producing the final output.