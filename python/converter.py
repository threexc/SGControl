
def converter(infile, outfile):
    i_data = []
    q_data = []
    total_data = []

    with open(infile, "r") as f:
        for line in f:
            total_data.append('{:.7f}'.format(float(line.rstrip('\n'))))
    f.close()
    #print(len(total_data))
    for value in range(0, len(total_data), 2):
        i_data.append(total_data[value])
        q_data.append(total_data[value+1])
    #print(total_data[0])
    #print(i_data[0])
    #print(i_data[1:10])
    #print(q_data[1:10])
    #print("BREAK")
    #print(q_data)

    with open(outfile, "w") as g:
        for point in range(0, len(i_data)):
            g.write("{0}, {1}\n".format(str(i_data[point]), str(q_data[point])))
    g.close()
