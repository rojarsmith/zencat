from cryptography import x509
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.serialization import \
    load_pem_private_key, load_pem_public_key
from cryptography.hazmat.backends import default_backend

# Load private key
with open("mock/server.key", "rb") as key_file:
    private_key = load_pem_private_key(
        key_file.read(),
        password = None,
        backend = default_backend()
    )

# Load certificate
with open("mock/server.crt", "rb") as cert_file:
        cert_data = cert_file.read()
        certificate = x509.load_pem_x509_certificate(cert_data, default_backend())

# Load public key
public_key = certificate.public_key()

with open("mock/fwa", "rb") as file:
    bin = file.read()

# Sign with private key
signature = private_key.sign(
    bin,
    padding.PSS(
        mgf = padding.MGF1(hashes.SHA256()),
        salt_length = padding.PSS.MAX_LENGTH
    ),
    hashes.SHA256()
)

print("Signature:", signature.hex())

pem_public_key = public_key.public_bytes(
    encoding = serialization.Encoding.PEM,
    format = serialization.PublicFormat.SubjectPublicKeyInfo
)
print("Public Key:\n", pem_public_key.decode('utf-8'))

# Validate with public key
try:
    public_key.verify(
        signature,
        bin,
        padding.PSS(
            mgf = padding.MGF1(hashes.SHA256()),
            salt_length = padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    print("The signature is valid.")
except Exception as e:
    print("The signature is invalid. Error:", str(e))

dbg = 0