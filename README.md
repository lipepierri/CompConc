<div align="center"> 
  <h1>CompConc - Trabalhos da Silvana BCC UFRJ 21.1</h1>
  <img src="/Imagens/logo.png" width="300"/>
 </div>

### Compilando...
Você pode abrir a Powershell com esse comando:

```Powershell
wsl code .
```

Compile e rode os programas em C acrescentando a opção -lpthread na linha de comando da forma abaixo
(a opção lpthread acrescenta as funções da biblioteca lpthread):

```Powershell
gcc fileName -o exitName -Wall -lpthread
./exitName args
```

Os programas em java deverão ser compilados e rodados dessa forma:
```Powershell
javac Main.java
java Main
```
