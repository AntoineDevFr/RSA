with open('test.txt', 'r') as file:
    content = file.read()
blocks = content.split(';')

for i in range(0, len(blocks) - 1, 4):  
    p = int(blocks[i], 16) 
    q = int(blocks[i + 1], 16)
    add = int(blocks[i + 2], 16)
    mult = int(blocks[i + 3], 16)
    
    print({p + q - add})
    print({p * q - mult})