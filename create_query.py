import random
query_count = 20
querylist_x = random.sample(range(0, 400), query_count)
querylist_y = random.sample(range(0, 400), query_count)
# print(querylist_x)
# print(querylist_y)
querylist = []
for i in range(query_count):
    querylist.append([querylist_x[i],querylist_y[i]])

# print(querylist)

f = open("query.csv","w")
for i in range(query_count):
    f.write(str(querylist_x[i])+" "+str(querylist_y[i])+"\n")
f.close()