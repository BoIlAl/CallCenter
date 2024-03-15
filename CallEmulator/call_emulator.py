import requests
import time

class Call_emulator:
    host = 'http://localhost:8080/'

    @staticmethod
    def emulate_one():
        req = requests.get(Call_emulator.host + 'call', {'number': 123})
        print(req.text)

    @staticmethod
    def emulate(emulate_time: float, interval: float):
        t = time()
        counter = 0
        while (emulate_time < time() - t):
            req = requests.get(Call_emulator.host + 'hi')
            req
            counter += 1
