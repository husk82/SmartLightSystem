import psycopg2
from .db_config import DB_CONFIG

def get_connection():
    try:
        conn = psycopg2.connect(**DB_CONFIG)  # type: ignore if Pyright complains
        return conn
    except Exception as e:
        print(f"DB connection failed: {e}")
        return None

def insert_lux(lux, timestamp):
    conn = get_connection()
    if not conn:
        return False
    try:
        with conn.cursor() as cur:
            cur.execute(
                "INSERT INTO lux_readings (lux_value, timestamp) VALUES (%s, %s)",
                (lux, timestamp)
            )
            conn.commit()
        return True
    except Exception as e:
        print(f"Insertion failed: {e}")
        return False
    finally:
        conn.close()

def fetch_last_readings(limit=10):
    conn = get_connection()
    if not conn:
        return []
    try:
        with conn.cursor() as cur:
            cur.execute("""
                SELECT lux_value, timestamp
                FROM lux_readings
                ORDER BY timestamp DESC
                LIMIT %s
            """, (limit,))
            rows = cur.fetchall()
        return [{"lux": row[0], "timestamp": row[1].strftime("%Y-%m-%d %H:%M:%S")} for row in rows]

    except Exception as e:
        print(f"Fetch failed: {e}")
        return []
    finally:
        conn.close()