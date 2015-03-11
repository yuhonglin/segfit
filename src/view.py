from matplotlib import pyplot as plt


viewcount = []
for l in open('./data/viewcount.test'):
    viewcount.append( map(int, l.split(' ')) )

videoID = []
for l in open('./data/videoID.test'):
    videoID.append( l.strip( ' \t\n' ) )


fig, axList = plt.subplots(3,1)

for i, vID in enumerate(videoID):

    # load viewcount
    vc = viewcount[i]

    # load fitted result
    segMatrix = []
    coefMatrix = []
    for l in open('./output/%s/%s/%s' % (vID[0], vID[1], vID)):
        tmp = l.split('\t')
        segMatrix.append( [int(tmp[0]), int(tmp[1])] )
        coefMatrix.append( [float(tmp[2]), float(tmp[3]), float(tmp[4])] )

    # plot viewcount
    axList[i].plot( range(0, len(vc)), vc, '.' )

    # plot segments
    maxVcRate = max(vc)/100.0
    for sm, cm in zip( segMatrix, coefMatrix ):
        segy = [ (cm[0]*pow(x, cm[1])+cm[2])*maxVcRate for x in range(1, sm[1]-sm[0]+2)]
        segx = range(sm[0]+1, sm[1]+2)

        axList[i].plot( segx, segy, linewidth=5 )
    
plt.show()

    
