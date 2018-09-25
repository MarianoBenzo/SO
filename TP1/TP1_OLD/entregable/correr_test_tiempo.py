import os

numThreads = [2, 4, 16, 32, 64]

def correr(ct, ca):
    path = './test-tiempo %d %d >> results.csv' % (ct, ca)
    if(os.system(path) == -1):
        print('ERROR EN EL ARCHIVO %s' % path)

if not os.path.exists("results.csv"):
    with open("results.csv", 'w') as f:
        f.write("ct,ca,tiempo,tiempo_c\n")

f = 100
for k in range(0, 10):
    for j in range(0, 100):
        for t in numThreads:
            correr(t, f)
