import random

def generate_2048_bit_number():
    """Génère un entier aléatoire de 2048 bits."""
    return random.getrandbits(2048)

# Exemple d'utilisation
a = generate_2048_bit_number()
b = generate_2048_bit_number()



print("\nNombre a :\n", hex(a))
print("\nNombre b :\n", hex(b))

print("\nA-B :\n", hex(a-b))
