# Microcontroladores-2026.2
Implementar comportamento de semáfores no microcontrolador ATMEGA328P

## Estrutura padrão de diretórios (projeto AVR)

```text
.
├── Makefile
├── README.md
├── include/
│   └── *.h
├── src/
│   ├── main.c
│   └── *.c
├── lib/
│   └── <biblioteca>/
│       ├── include/
│       │   └── *.h
│       └── src/
│           └── *.c
├── tests/
│   └── *.c
├── docs/
│   └── *.md
└── build/
    ├── obj/
    └── bin/
```

- `src/`: código-fonte principal da aplicação AVR.
- `include/`: arquivos de cabeçalho globais do projeto.
- `lib/`: bibliotecas internas reutilizáveis (cada uma com `include/` e `src/`).
- `tests/`: testes unitários/integrados, quando aplicável.
- `docs/`: documentação técnica e instruções de uso.
- `build/`: artefatos gerados no processo de compilação (não versionar).
- `Makefile`: automação de compilação, gravação e limpeza do projeto.
