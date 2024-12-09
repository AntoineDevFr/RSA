import random

def generate_2048_bit_number():
    """Génère un entier aléatoire de 2048 bits."""
    return random.getrandbits(2048)

def generate_4096_bit_number():
    """Génère un entier aléatoire de 2048 bits."""
    return random.getrandbits(4096)

def generate_modulo_numbers():
    """Génère deux nombres aléatoires de 2048 bits modulo un N aléatoire."""
    # Générer N (le modulo), a et b
    N = generate_4096_bit_number()
    a = generate_4096_bit_number()
    b = generate_4096_bit_number()

    # Réduire a et b modulo N
    a_mod = a % N
    b_mod = b % N

    return N, a_mod, b_mod

# Exemple d'utilisation
N, a, b = generate_modulo_numbers()

print("Modulo N (2048 bits) :\n", hex(N))
print("\nNombre a (2048 bits mod N) :\n", hex(a))
print("\nNombre b (2048 bits mod N) :\n", hex(b))

print("\nA+B modulo n:\n", hex((a+b)%N))
