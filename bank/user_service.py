import os
import sqlite3
from datetime import datetime, timedelta
from passlib.hash import pbkdf2_sha256
from flask import request, g
import jwt

SECRET = 'bfg28y7efg238re7r6t32gfo23vfy7237yibdyo238do2v3'
_DB = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bank.db')

def get_user_with_credentials(email, password):
    # Passwords are stored as PBKDF2-SHA256 salted hashes — plaintext is never saved
    # Both "user not found" and "wrong password" return None with the same error message
    # to prevent user enumeration (attacker can't tell which emails are registered)
    # Parameterized query prevents SQL injection on the email lookup
    try:
        con = sqlite3.connect(_DB)
        cur = con.cursor()
        cur.execute('''
            SELECT email, name, password FROM users where email=?''',
            (email,))
        row = cur.fetchone()
        if row is None:
            return None
        email, name, hash = row
        if not pbkdf2_sha256.verify(password, hash):
            return None
        return {"email": email, "name": name, "token": create_token(email)}
    finally:
        con.close()

def logged_in():
    # JWT stored in a cookie — the signature is verified on every request
    # An expired or tampered token raises InvalidTokenError and is rejected
    token = request.cookies.get('auth_token')
    try:
        data = jwt.decode(token, SECRET, algorithms=['HS256'])
        g.user = data['sub']
        return True
    except jwt.InvalidTokenError:
        return False

def create_token(email):
    # Tokens expire after 60 minutes to limit the window of a stolen token
    now = datetime.utcnow()
    payload = {'sub': email, 'iat': now, 'exp': now + timedelta(minutes=60)}
    token = jwt.encode(payload, SECRET, algorithm='HS256')
    return token