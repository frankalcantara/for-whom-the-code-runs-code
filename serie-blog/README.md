# Série do blog: Programação Competitiva em C++23

Programas dos artigos da série *Programação Competitiva em C++23*, a versão web em português do livro, publicada em [frankalcantara.com](https://frankalcantara.com). Cada pasta `competitiva-NN` corresponde ao artigo de número `NN` da série.

## Conteúdo de cada pasta

- `*.cpp`: programas completos em C++23, com comentários em português.
- `*.in`: entradas de teste, lidas pela entrada padrão. Um programa `nome.cpp` pode ter os casos `nome.in`, `nome.1.in`, `nome.2.in` e assim por diante.
- `*.esperado`: a saída correta de cada caso, ao lado da entrada correspondente. Linhas que começam com `~` são expressões regulares, usadas para valores que variam de máquina para máquina, como tempos medidos. Um `nome.esperado` sem `nome.in` é a saída de um programa que não lê nada.
- `*.hpp`: cabeçalhos auxiliares do artigo, como `medicao.hpp`, com o protocolo de medição (aquecimento, cinco rodadas e mediana), e `leitor.hpp`, com o leitor rápido de inteiros.

Casos de teste com mais de 500 KB não estão neste repositório.

## Compilador de referência

Todos os programas foram compilados, executados e conferidos com o MSVC:

```text
Visual Studio 18.10.3
Microsoft C/C++ Optimizing Compiler 19.51.36260 for x64
cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /utf-8
```

Os tempos citados nos artigos foram medidos com esse compilador em um Intel Core i7-10750H.

## Observações de portabilidade

A maioria dos programas também compila com LLVM e libc++ recentes. Alguns usam recursos do C++23 que nem todas as bibliotecas oferecem ainda:

```text
competitiva-04/04-generico.cpp    parâmetro de objeto explícito (deducing this)
competitiva-04/05-lambdas.cpp     formatação de intervalos com std::println
competitiva-05/03-matrizes.cpp    std::mdspan
```

O programa `competitiva-07/08-sala-dos-mapas.cpp` usa `MapViewOfFile` no Windows e recai para a leitura comum nos outros sistemas.
