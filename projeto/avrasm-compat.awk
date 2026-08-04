# Conversão transparente de sintaxe AVRASM2 (Microchip Studio) para GNU as.
#
# Aplicada pelo Makefile a cada build, com o arquivo passado duas vezes: a
# primeira passada anota os rótulos do programa, a segunda faz a conversão.
# Cada linha de entrada gera exatamente uma linha de saída, para que o debug
# (breakpoints, step) aponte sempre para a linha certa do seu .asm original.
#
# Duas diferenças de fundo entre os montadores são resolvidas aqui:
#
#   * segmentos — o .org conta words no segmento de código e bytes nos demais
#     no AVRASM2; no GNU as é sempre byte, a partir do início da seção.
#
#   * rótulos de código — valem endereços de WORD no AVRASM2 e de BYTE no
#     GNU as. Por isso "low(tabela*2)" (idioma de LPM) vira "lo8(tabela)", e
#     "low(rotina)" (idioma de IJMP/ICALL) vira "pm_lo8(rotina)".

# ---------------------------------------------------------------------------
# Primeira passada: anota os rótulos do segmento de código (só eles valem
# endereços de word no AVRASM2; rótulos de dados e EEPROM já são bytes).
# ---------------------------------------------------------------------------
BEGIN { seg1 = "c" }

FNR == NR {
    minus1 = tolower($0)
    if (minus1 ~ /^[ \t]*\.cseg([ \t;]|$)/) seg1 = "c"
    else if (minus1 ~ /^[ \t]*\.dseg([ \t;]|$)/) seg1 = "d"
    else if (minus1 ~ /^[ \t]*\.eseg([ \t;]|$)/) seg1 = "e"

    if (seg1 == "c" && match($0, /^[ \t]*[A-Za-z_][A-Za-z_0-9]*:/)) {
        rotulo = substr($0, RSTART, RLENGTH)
        sub(/^[ \t]*/, "", rotulo)
        sub(/:$/, "", rotulo)
        eh_rotulo[rotulo] = 1
    }
    next
}

# ---------------------------------------------------------------------------
# Converte high()/low() levando em conta se o argumento é um rótulo.
# ---------------------------------------------------------------------------
function converte_hi_lo(s,   saida, antes, alto, arg, resto, i, c, prof, nome) {
    saida = ""
    while (match(s, /[Hh][Ii][Gg][Hh][ \t]*\(|[Ll][Oo][Ww][ \t]*\(/)) {
        antes = substr(s, 1, RSTART - 1)
        alto  = (tolower(substr(s, RSTART, 4)) == "high")
        s     = substr(s, RSTART + RLENGTH)     # texto após o '('

        # recorta o argumento até o parêntese que fecha
        arg = ""; prof = 1; i = 1
        while (i <= length(s)) {
            c = substr(s, i, 1)
            if (c == "(") prof++
            else if (c == ")") { prof--; if (prof == 0) break }
            arg = arg c
            i++
        }
        resto = substr(s, i + 1)

        nome = arg
        gsub(/[ \t]/, "", nome)

        if (match(nome, /^[A-Za-z_][A-Za-z_0-9]*\*2$/) && \
            eh_rotulo[substr(nome, 1, length(nome) - 2)]) {
            # low(tabela*2): o AVRASM2 converte word->byte; aqui já é byte
            nome = substr(nome, 1, length(nome) - 2)
            saida = saida antes (alto ? "hi8(" : "lo8(") nome ")"
        } else if (nome in eh_rotulo) {
            # low(rotina): o AVRASM2 dá o endereço em words
            saida = saida antes (alto ? "pm_hi8(" : "pm_lo8(") nome ")"
        } else {
            saida = saida antes (alto ? "hi8(" : "lo8(") arg ")"
        }
        s = resto
    }
    return saida s
}

# ---------------------------------------------------------------------------
# Segunda passada: a conversão em si.
# ---------------------------------------------------------------------------
{
    linha = $0
    minus = tolower(linha)

    # ---- troca de segmento -------------------------------------------------
    if (minus ~ /^[ \t]*\.cseg([ \t;]|$)/) { seg = "c"; print "\t.text"; next }
    if (minus ~ /^[ \t]*\.dseg([ \t;]|$)/) { seg = "d"; print "\t.section .bss"; next }
    if (minus ~ /^[ \t]*\.eseg([ \t;]|$)/) { seg = "e"; print "\t.section .eeprom"; next }

    # ---- .org --------------------------------------------------------------
    if (minus ~ /^[ \t]*\.org[ \t]+/) {
        comentario = ""
        pos = index(linha, ";")
        if (pos > 0) { comentario = " " substr(linha, pos); linha = substr(linha, 1, pos - 1) }
        sub(/^[ \t]*\.[Oo][Rr][Gg][ \t]+/, "", linha)
        sub(/[ \t]+$/, "", linha)

        if (seg == "d")
            print "\t.org (" linha ")-0x100" comentario     # SRAM começa em 0x100
        else if (seg == "e")
            print "\t.org (" linha ")" comentario
        else
            print "\t.org (" linha ")*2" comentario         # código: word -> byte
        next
    }

    # ---- reserva de memória ------------------------------------------------
    # No AVRASM2, ".byte N" (válido só no .dseg) reserva N bytes; no GNU as
    # essa diretiva emite dados, e quem reserva é o .space. Precisa vir antes
    # da conversão de .db, para não capturar o resultado dela. A diretiva
    # costuma vir logo depois de um rótulo ("contador: .byte 1").
    if (minus ~ /^[ \t]*([a-z_][a-z_0-9]*:)?[ \t]*\.byte[ \t]+/) {
        sub(/\.[Bb][Yy][Tt][Ee]/, ".space", linha)
        print linha
        next
    }

    # ---- diretivas com sintaxe diferente -----------------------------------
    # .equ NOME = VALOR  ->  .equ NOME, VALOR   (idem .set)
    if (minus ~ /^[ \t]*\.equ[ \t]+/ || minus ~ /^[ \t]*\.set[ \t]+/)
        sub(/[ \t]*=[ \t]*/, ", ", linha)

    # .def ALIAS = rN -> #define ALIAS rN   |   .undef ALIAS -> #undef ALIAS
    # (o comentário sai da linha: ele viraria parte do texto da macro)
    if (minus ~ /^[ \t]*\.def[ \t]+/) {
        sub(/\.[Dd][Ee][Ff][ \t]+/, "#define ", linha)
        sub(/[ \t]*=[ \t]*/, " ", linha)
        sub(/[ \t]*;.*$/, "", linha)
    }
    else if (minus ~ /^[ \t]*\.undef[ \t]+/) {
        sub(/\.[Uu][Nn][Dd][Ee][Ff][ \t]+/, "#undef ", linha)
        sub(/[ \t]*;.*$/, "", linha)
    }

    # .device é dispensável aqui (o alvo vem do -mmcu no build)
    sub(/\.[Dd][Ee][Vv][Ii][Cc][Ee]/, "; .device", linha)

    # ---- dados no segmento de código ---------------------------------------
    sub(/\.[Dd][Bb][ \t]/, ".byte ", linha)

    if (sub(/\.[Dd][Ww][ \t]/, ".word ", linha)) {
        # ".dw rotulo" é tabela de endereços de código: no AVRASM2 os rótulos
        # já valem words; no GNU as pedimos isso com gs().
        nlista = split(substr(linha, index(linha, ".word ") + 6), itens, ",")
        prefixo = substr(linha, 1, index(linha, ".word ") + 5)
        nova = ""
        for (k = 1; k <= nlista; k++) {
            item = itens[k]
            limpo = item
            gsub(/[ \t]/, "", limpo)
            sub(/;.*$/, "", limpo)
            if (limpo in eh_rotulo)
                sub(limpo, "gs(" limpo ")", item)
            nova = nova (k > 1 ? "," : "") item
        }
        linha = prefixo nova
    }

    # ---- expressões --------------------------------------------------------
    gsub(/\$[0-9a-fA-F]+/, "0x&", linha)      # $FF  -> 0x$FF ...
    gsub(/0x\$/, "0x", linha)                 #      -> 0xFF
    linha = converte_hi_lo(linha)

    # "rjmp PC" (trava a execução no AVRASM2) -> "rjmp ."
    if (minus ~ /^[ \t]*rjmp[ \t]+pc[ \t]*(;.*)?$/)
        sub(/[Pp][Cc]/, ".", linha)

    print linha
}
