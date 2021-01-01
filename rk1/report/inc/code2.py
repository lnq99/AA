RANGE = 8

def randSymMatrix(rank, density=0.2, format='coo', dtype=np.int8):
    m = scipy.sparse.rand(rank, rank, density=density, format=format, dtype=dtype)
    m = m.todense() % RANGE
    return m


def randLinearPattern(rank, density=0.01, center=1.05, dtype=np.int8):
    m = randSymMatrix(rank, density, dtype=dtype)
    for i in range(rank):
        for j in range(i+1):
            if i == j:
                if not m[i,j]:
                    m[i,j] = randint(1,RANGE)
            else:
                if not randint(0, int((i-j)**center)):
                    m[i,j] = randint(1,RANGE)
    return m


def toJennings(m):
    AN = []
    IA = []
    try: m = m.tolist()
    except: pass
    l = len(m[0])
    for i in range(l):
        for j in range(i+1):
            if m[i][j]:
                AN += m[i][j:i+1]
                IA.append(len(AN))
                break
    return np.array(AN), np.array(IA)


def jenningsToMatrix(AN, IA):
    m = []
    l = len(IA)
    start = 0
    for i in range(l):
        end = IA[i]
        a = [0]*(i+1+start-end) + AN[start:end] + [0]*(l-i-1)
        m.append(a)
        start = end
    return np.matrix(m)


# change source matrix
def toHalfMatrix(m):
    l = m.shape[0]
    for i in range(l-1):
        for j in range(i+1, l):
            m[i,j] = 0
    return m


# change source matrix
def toFullMatrix(m):
    l = m.shape[0]
    for i in range(l-1):
        for j in range(i+1, l):
            m[i,j] = m[j,i]
    return m


def plotMatrix(m):
    try: toFullMatrix(m)
    except: pass
    plt.imshow(m, interpolation='none', cmap='binary')
    plt.colorbar()


def toJenningsFile(path, m):
    AN, IA = toJennings(m)
    with open(path, 'w') as f:
        f.write(','.join(map(str, AN)))
        f.write('\n')
        f.write(','.join(map(str, IA)))
        f.write('\n')
        f.write(str(np.sum(m, axis=0).argmax()))