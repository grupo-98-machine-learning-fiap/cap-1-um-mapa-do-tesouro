# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# Cap 1 - Um Mapa do Tesouro

## Grupo 98 

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/company/inova-fusca">Eduardo</a> 
- <a href="https://www.linkedin.com/company/inova-fusca">Francisco Bittencourt</a>
- <a href="https://www.linkedin.com/in/kaiquecadimiel/">Kaique Cadimiel</a>

## 👩‍🏫 Professores:
### Tutor(a) 
- <a href="https://www.linkedin.com/company/inova-fusca">Nome do Tutor</a>
### Coordenador(a)
- <a href="https://www.linkedin.com/company/inova-fusca">Nome do Coordenador</a>


## 📜 Descrição 

Projeto: Sistema de Irrigação Inteligente – FarmTech Solutions

Visão Geral
Este projeto integra IoT, Backend e Agricultura Digital para otimizar o uso de recursos hídricos e nutricionais em lavouras. O foco é o desenvolvimento de um dispositivo baseado no ESP32 que monitora as condições do solo e do ambiente, automatizando o acionamento de uma bomba d'água conforme a necessidade específica de uma cultura agrícola.

Funcionalidades Principais
Monitoramento de Nutrientes (NPK): Simulação dos níveis de Nitrogênio, Fósforo e Potássio através de botões físicos (lógica booleana).

Controle de pH: Utilização de sensor LDR para representar a acidez/alcalinidade do solo, mapeando a intensidade de luz para uma escala de pH (0 a 14).

Sensoriamento de Umidade: Monitoramento em tempo real da umidade do solo utilizando o sensor DHT22.

Automação de Irrigação: Acionamento inteligente de um Relé (representando a bomba d'água) baseado no cruzamento dos dados de sensores.

Integração Meteorológica (Opcional): Script em TypeScript para consulta da API OpenWeather, permitindo suspender a irrigação em caso de previsão de chuva.

Stack Técnica
Simulador: Wokwi

Hardware: ESP32, Relé, Sensores (DHT22, LDR, Botões)

Linguagens: C++ (Firmware), TypeScript (Integração de APIs), R (Análise de Dados)

Documentação: GitHub e Jira

Nota: Esta solução foi desenvolvida como parte da Fase 2 do projeto acadêmico na FIAP, visando a eficiência operacional da startup FarmTech Solutions.


## 📁 Estrutura de pastas

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- <b>.github</b>: Nesta pasta ficarão os arquivos de configuração específicos do GitHub que ajudam a gerenciar e automatizar processos no repositório.

- <b>assets</b>: aqui estão os arquivos relacionados a elementos não-estruturados deste repositório, como imagens.

- <b>config</b>: Posicione aqui arquivos de configuração que são usados para definir parâmetros e ajustes do projeto.

- <b>document</b>: aqui estão todos os documentos do projeto que as atividades poderão pedir. Na subpasta "other", adicione documentos complementares e menos importantes.

- <b>scripts</b>: Posicione aqui scripts auxiliares para tarefas específicas do seu projeto. Exemplo: deploy, migrações de banco de dados, backups.

- <b>src</b>: Todo o código fonte criado para o desenvolvimento do projeto ao longo das 7 fases.

- <b>README.md</b>: arquivo que serve como guia e explicação geral sobre o projeto (o mesmo que você está lendo agora).

## 🔧 Como executar o código

*Acrescentar as informações necessárias sobre pré-requisitos (IDEs, serviços, bibliotecas etc.) e instalação básica do projeto, descrevendo eventuais versões utilizadas. Colocar um passo a passo de como o leitor pode baixar o seu código e executá-lo a partir de sua máquina ou seu repositório. Considere a explicação organizada em fase.*

📎 Clonar o projeto
```shell
git clone git@github.com:grupo-98-machine-learning-fiap/cap-1-um-mapa-do-tesouro.git
```


## 🗃 Histórico de lançamentos

* 0.5.0 - XX/XX/2024
    * 
* 0.4.0 - XX/XX/2024
    * 
* 0.3.0 - XX/XX/2024
    * 
* 0.2.0 - XX/XX/2024
    * 
* 0.1.0 - XX/XX/2024
    *

## Link da atividade
```Shell
https://on.fiap.com.br/mod/assign/view.php?id=602997&returnto=conteudocurso
```

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>


