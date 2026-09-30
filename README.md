# Microcontroladores-2026.2
Implementar comportamento de semáfores no microcontrolador ATMEGA328P

## Estrutura padrão de diretórios (projeto AVR em Assembly)

```text
.
├── Makefile
├── README.md
├── include/
│   └── *.inc
├── src/
│   ├── main.asm
│   └── *.asm
├── lib/
│   └── <biblioteca>/
│       ├── include/
│       │   └── *.inc
│       └── src/
│           └── *.asm
├── tests/
│   └── *.asm
├── docs/
│   └── *.md
└── build/
    ├── obj/
    └── bin/
```

- `src/`: código-fonte principal em Assembly da aplicação AVR.
- `include/`: arquivos de inclusão (`.inc`) globais do projeto.
- `lib/`: bibliotecas internas reutilizáveis em Assembly (cada uma com `include/` e `src/`).
- `tests/`: testes em Assembly, quando aplicável.
- `docs/`: documentação técnica e instruções de uso.
- `build/`: artefatos gerados no processo de compilação (não versionar).
- `Makefile`: automação de montagem, gravação e limpeza do projeto.
