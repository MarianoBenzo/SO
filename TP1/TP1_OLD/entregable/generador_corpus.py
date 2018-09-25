import random
random.seed(0)

cant_a_generar = 100

with open("corpus", 'r') as f:
    lineas = f.readlines()
    # Saco el newline de cada linea
    # lineas = [i[:-1] for i in lineas]
   
    cantidadTotal = 0
    b = {}

    for j in range(0, cant_a_generar):
        i = random.randint(0, len(lineas))
        a = random.sample(lineas, i)

        cantidadTotal += len(a)
        for i in a:
        	if i not in b.keys():
        		b[i] = 1
        	else:
        		b[i] += 1

        with open("casos_tests_corpus/corpus_%d" % j, 'w') as out:
            for l in a:
                out.write(l)

    with open("casos_tests_corpus/corpus_sol", 'w') as out:
        out.write(str(cantidadTotal))
        out.write('\n')
        out.write(str(max(b.values())))
