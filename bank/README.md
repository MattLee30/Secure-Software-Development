# Secure Banking App

A Flask-based banking application built as a code-along for CMSI 662 Web Security.

## Setup

```bash
python3 -m venv env
. env/bin/activate
pip install Flask Flask-WTF PyJWT passlib
python bin/createdb.py
python bin/makeaccounts.py
flask run
```

Login at `http://localhost:5000` with:
- `alice@example.com` / `123456`
- `bob@example.com` / `123456`

## Security features implemented

- **XSS prevention** — all user-supplied values rendered through Jinja2 templates, which auto-escape HTML
- **SQL injection prevention** — every database query uses parameterized statements (no string interpolation)
- **CSRF protection** — Flask-WTF injects and verifies a signed token on every POST form
- **Password storage** — PBKDF2-SHA256 salted hashes via `passlib`; plaintext is never stored
- **Session tokens** — short-lived JWTs (60 min) stored in cookies and verified server-side on every request
- **Authorization** — account balance lookups always join on `owner = <logged-in user>`; users cannot read or transfer from other users' accounts