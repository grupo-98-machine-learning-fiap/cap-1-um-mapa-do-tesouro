# Documentacao do Codigo Wokwi

Este diretorio contem o firmware do projeto de irrigacao inteligente executado no simulador Wokwi com ESP32.

## Visao Geral

O codigo em [src/sketch.ino](src/sketch.ino) monitora:

- Umidade e temperatura pelo DHT22.
- pH simulado por leitura analogica do LDR.
- Disponibilidade de nutrientes N, P e K por 3 botoes.

Com base nessas entradas, o sistema decide ligar ou nao a irrigacao (rele) e acender um LED de status.

## Arquivos Principais

- [src/sketch.ino](src/sketch.ino): firmware principal (logica de leitura, validacao e decisao).
- [src/diagram.json](src/diagram.json): circuito do Wokwi com componentes e conexoes.
- [src/wokwi-project.txt](src/wokwi-project.txt): referencia do projeto no Wokwi.
- [src/libraries.txt](src/libraries.txt): biblioteca externa usada (DHT).

## Componentes Simulados

- ESP32 DevKit C V4
- Sensor DHT22
- Sensor LDR (saida analogica)
- 3 botoes (N, P e K)
- Modulo rele
- LED + resistor de 330 ohms

## Mapeamento de Pinos

| Componente | Pino no ESP32 | Uso |
|---|---:|---|
| DHT22 (SDA) | GPIO 4 | Leitura de umidade e temperatura |
| LDR (AO) | GPIO 34 | Leitura analogica para simulacao de pH |
| Botao N | GPIO 18 | Ativa/desativa nutriente N |
| Botao P | GPIO 19 | Ativa/desativa nutriente P |
| Botao K | GPIO 21 | Ativa/desativa nutriente K |
| Rele (IN) | GPIO 23 | Acionamento da bomba de irrigacao |
| LED indicador | GPIO 2 | Indica irrigacao ligada |

## Regras de Negocio

O sistema so liga a irrigacao quando todas as condicoes abaixo sao verdadeiras:

1. Solo seco: umidade < 40%
2. pH adequado: entre 5.5 e 6.5
3. Nutrientes suficientes: pelo menos 2 entre N, P, K ativos

Se qualquer condicao falhar, a irrigacao permanece desligada.

## Parametros de Controle

Definidos em [src/sketch.ino](src/sketch.ino):

- Umidade seca: 40.0
- pH minimo: 5.5
- pH maximo: 6.5
- Nutrientes minimos: 2
- Debounce dos botoes: 50 ms
- Intervalo entre leituras: 2000 ms

## Fluxo do Firmware

1. Inicializa serial, DHT22, botoes, rele e LED.
2. Executa debounce dos botoes NPK continuamente.
3. A cada 2 segundos:
	1. Le umidade e temperatura.
	2. Le LDR e converte para pH na escala 0 a 14.
	3. Conta nutrientes ativos (NPK).
	4. Aplica regras de decisao.
	5. Liga/desliga rele e LED.
4. Escreve o diagnostico completo no monitor serial.

## Conversao ADC para pH

A funcao abaixo converte o ADC do ESP32 para pH:

$$
pH = \frac{4095 - ADC}{4095} \times 14
$$

Interpretacao:

- ADC alto tende a pH menor.
- ADC baixo tende a pH maior.

## Debounce dos Botoes

Cada botao N/P/K usa debounce por tempo:

- Ao detectar mudanca de leitura, o firmware inicia janela de 50 ms.
- Se o estado permanecer estavel apos esse tempo, confirma o clique.
- O clique alterna o estado logico do nutriente (ON/OFF).

Isso evita multiplos acionamentos por ruido mecanico do botao.

## Como Simular no Wokwi

1. Abra o projeto indicado em [src/wokwi-project.txt](src/wokwi-project.txt).
2. Inicie a simulacao.
3. Abra o monitor serial.
4. Teste os cenarios:
	1. Ajuste umidade do DHT22 para abaixo de 40%.
	2. Ajuste o LDR para manter pH entre 5.5 e 6.5.
	3. Clique em pelo menos 2 botoes entre N/P/K.
5. Verifique se a mensagem de irrigacao ativada aparece e o rele/LED ligam.

## Mensagens Esperadas no Serial

Durante a execucao, o monitor serial mostra:

- Estado da umidade (seco/umido)
- Temperatura atual
- pH calculado (adequado/inadequado)
- Estado de N, P, K (ON/OFF)
- Decisao final da irrigacao

Exemplos de saida:

- Solo umido. Irrigacao desligada.
- Solo seco, verificando...
- Irrigacao ATIVADA.
- Irrigacao bloqueada: pH inadequado.

## Dependencias

Biblioteca usada no firmware:

- DHT sensor library

Registrada em [src/libraries.txt](src/libraries.txt).

## Possiveis Melhorias

- Substituir simulacao de pH por sensor de pH real.
- Adicionar histerese para reduzir liga/desliga frequente.
- Enviar telemetria para backend (MQTT/HTTP).
- Registrar historico de leituras em banco de dados.
