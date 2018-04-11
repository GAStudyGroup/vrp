# Vehicle Routing Problem implementation using the Generalized Partition Crossover

### Muito Importante

- Função de balanceamento

valgrind  --tool=memcheck --leak-check=yes --log-file=valgrind_output.txt -v ./bin/GA
--leak-check=full --show-leak-kinds=all

### Necessário

- Encontrar um método de perturbação mais eficiente (se houver)

- Encontrar uma função de fitness mais eficiente

### Adicional

- Colocar os parametros

- Colocar Threads quando for rodar a mutação

- Ver o que dá pra otimizar dentro da  mutação

# Idéias

- Pop inicial: gerar completamente random, colocar a reparação e a mutação em seguida

- Logs para comparar ações do GPX e da mutação, separadamente (igual para o OX)

## Arguments

  Code     | Alias   | What is                       | Required   
 :-----:   | :-----: | :----------------:            | :--------:   
 `-name`   | `-n`    | Name of .vrp file             | **Yes**           
 `-size`   | `-s`    | Population Size               | **Yes**
 `-trucks` | `-t`    | Truck number                  | **Yes**
 `-it`     | -       | Max limit of iterations       | **Yes**            
 `-id`     | -       | ID of actual Run (Log)        | **No**
 `-cross`  | `-cx`   | Crossover method (1-2)        | **No**
 `-path`   | -       | Path of .vrp folder           | **No**
 `-fit`    | -       | Fitness method   (1-2)        | **No**
 `-mrate`  | -       | Mutation Rate (0-100          | **No**
 `-initm`  | -       | Initial Mutation It...(0-100) | **No**


### Site 
http://vrp.atd-lab.inf.puc-rio.br/index.php/en/

### Instâncias de VRP
http://neo.lcc.uma.es/vrp/vrp-instances/capacitated-vrp-instances/
### Melhores resultados
http://neo.lcc.uma.es/vrp/known-best-results/
