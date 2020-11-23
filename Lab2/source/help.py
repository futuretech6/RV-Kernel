
for i in range(1, 32):
    print("sd x"+str(i)+", "+str(8*(33-i))+"(sp)")
print()
for i in range(31, 0, -1):
    print("ld x"+str(i)+", "+str(8*(33-i))+"(sp)")