import psycopg2
from dbConfig import DB_CONFIG

def create_table():
    try:
        conn = psycopg2.connect(**DB_CONFIG)
        with conn.cursor() as cur:
            cur.execute("""
                CREATE TABLE IF NOT EXIST lux_readings (
                    id SERIAL PRIMARY KEY,
                    lux_value FLOAT NOT NULL,
                    timestamp TIMESTAMP NOT NULL
                )
                """)
            conn.commit()
        print ("Table 'lux_readings' created successfully")
    except Exception as e:
        print("Error creating table: {e}")
    finally:
        if conn:
              conn.close()

if __name__ == "__main__"
    create_table()

