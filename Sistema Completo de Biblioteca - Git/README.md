# Sistema de Gerenciamento de Empréstimos de Livros

## Descrição
Sistema desenvolvido em C++ para gerenciar empréstimos de livros em uma biblioteca escolar ou universitária. O sistema permite cadastrar livros e alunos, além de controlar empréstimos e devoluções com persistência de dados em arquivos CSV.

## Funcionalidades

O sistema oferece as seguintes funcionalidades através de um menu interativo:

1. **Cadastrar Livro** - Adiciona um novo livro ao sistema
2. **Cadastrar Aluno** - Adiciona um novo aluno ao sistema
3. **Editar Livro** - Edita informações de um livro registrado no sistema
4. **Editar Aluno** - Edita informações de um aluno registrado no sistema
5. **Listar Livros** - Exibe todos os livros cadastrados com seus status (disponível/emprestado)
6. **Listar Alunos** - Exibe todos os alunos cadastrados
7. **Listar Empréstimos** - Mostra todos os empréstimos (abertos e devolvidos)
8. **Registrar Empréstimo** - Registra um novo empréstimo de livro
9. **Registrar Devolução** - Registra a devolução de um livro emprestado
10. **Apagar Livro** - Remove um livro do sistema (apenas se não estiver emprestado)
11. **Apagar Aluno** - Remove um aluno do sistema

## Estrutura de Dados

### Livro
- **codigo** (string): Código/ISBN do livro
- **titulo** (string): Título do livro
- **autor** (string): Nome do autor
- **ano_publicacao** (int): Ano de publicação

### Aluno
- **matricula** (string): Matrícula única do aluno
- **nome** (string): Nome completo
- **curso** (string): Curso do aluno
- **email** (string): E-mail de contato

### Empréstimo
- **codigo_emprestimo** (int): Código sequencial único
- **matricula_aluno** (string): Referência ao aluno
- **codigo_livro** (string): Referência ao livro
- **data_emprestimo** (string): Data no formato AAAA-MM-DD
- **data_devolucao** (string): Data no formato AAAA-MM-DD (vazio se não devolvido)
- **status** (string): "Aberto" ou "Devolvido"

## Arquivos CSV

O sistema utiliza três arquivos CSV para persistência:

- **aluno.csv** - Armazena os dados dos alunos
- **livro.csv** - Armazena os dados dos livros
- **emprestimo.csv** - Armazena os dados dos empréstimos

Os arquivos são criados automaticamente na primeira execução se não existirem.

## Como Compilar e Executar

### No Windows (usando MinGW ou similar):
```bash
g++ -o sistema_biblioteca main.cpp -std=c++11
sistema_biblioteca.exe
```

### No Linux/Mac:
```bash
g++ -o sistema_biblioteca main.cpp -std=c++11
./sistema_biblioteca
```

### Usando Code::Blocks, Dev-C++ ou Visual Studio:
1. Crie um novo projeto
2. Adicione o arquivo `main.cpp` ao projeto
3. Compile e execute (F9 ou Ctrl+F9)

## Exemplos de Uso

### Cadastrando um Livro
```
Escolha uma opcao: 1
Codigo (ISBN): 9788521611905
Titulo: Estruturas de Dados
Autor: Niklaus Wirth
Ano de Publicacao: 2010

- Livro cadastrado com sucesso!
```

### Cadastrando um Aluno
```
Escolha uma opcao: 2
Matricula: 202301234
Nome: Maria Silva
Curso: Ciencia da Computacao
Email: maria.silva@email.com

- Aluno cadastrado com sucesso!
```
### Editando um Livro
```
Escolha uma opcao: 7
Digite o codigo do livro a ser editado: 9788521611905

--- Editando Livro ---
Dados atuais:
Codigo: 9788521611905
Titulo: Estruturas de Dados
Autor: Niklaus Wirth
Ano: 2010
-------------------
Insira o novo titulo: Estruturas de Dados e Algoritmos

- Livro editado com sucesso!
```
### Editando um Aluno
```
Escolha uma opcao: 3
Digite a matricula do aluno a ser editado: 202301234

--- Editando Aluno ---
Dados atuais:
Matricula: 202301234
Nome: Maria Silva
Curso: Ciencia da Computacao
Email: maria.silva@email.com
-------------------
Insira o novo nome: Maria Silva Santos

- Aluno editado com sucesso!
```

### Registrando um Empréstimo
```
Escolha uma opcao: 6
Matricula do aluno: 202301234
Codigo do livro: 9788521611905

- Emprestimo registrado com sucesso!
Codigo do emprestimo: 1
Data do emprestimo: 2025-11-04
```

### Registrando uma Devolução
```
Escolha uma opcao: 7
Codigo do livro a ser devolvido: 9788521611905

- Devolucao registrada com sucesso!
Data da devolucao: 2025-11-10
```

## Validações Implementadas

O sistema inclui as seguintes validações:

- ✅ Impede cadastro de livros/alunos com códigos/matrículas duplicados
- ✅ Verifica se o aluno existe antes de registrar empréstimo
- ✅ Verifica se o livro existe antes de registrar empréstimo
- ✅ Impede emprestar um livro já emprestado
- ✅ Impede apagar livros que estão emprestados
- ✅ Gera códigos de empréstimo sequenciais automaticamente
- ✅ Registra automaticamente a data atual nos empréstimos/devoluções
- ✅ Impede apagar alunos que têm empréstimos ativos
- ✅ Valida entrada de dados numéricos (ano de publicação)

## Estrutura do Código

O código está organizado em seções:

1. **Estruturas de Dados** - Definição dos structs
2. **Funções Auxiliares** - Funções utilitárias (split, limparTela, etc)
3. **Funções de Alunos** - CRUD de alunos
4. **Funções de Livros** - CRUD de livros
5. **Funções de Empréstimos** - Gerenciamento de empréstimos
6. **Menu Principal** - Interface do usuário

## Observações Importantes

- Os campos `codigo` (livro) e `matricula` (aluno) são do tipo **string** conforme especificado
- O sistema usa o separador `;` (ponto e vírgula) nos arquivos CSV
- A data é obtida automaticamente do sistema operacional
- Os arquivos CSV são criados automaticamente se não existirem
- O sistema limpa a tela entre operações para melhor usabilidade
- A função de edição de aluno permite mudar apenas o nome do aluno
- A função de edição de livro permite mudar apenas o título do livro

## Requisitos

- Compilador C++ com suporte a C++11 ou superior
- Sistema operacional: Windows, Linux ou Mac
- Nenhuma biblioteca externa necessária (usa apenas STL)

## Troubleshooting

**Problema:** Erro ao compilar
- **Solução:** Certifique-se de usar a flag `-std=c++11` ao compilar

**Problema:** Arquivos CSV não são criados
- **Solução:** Verifique se o programa tem permissão de escrita na pasta

**Problema:** Caracteres acentuados aparecem incorretos
- **Solução:** Configure o terminal para UTF-8 ou evite usar acentos nos dados
