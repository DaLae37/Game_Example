import gym
import socket
import struct
import numpy as np

HOST = '127.0.0.1'
PORT = 5050

class custom_env(gym.Env):
    def __init__(self, id):
        self.id = id #multi env에서 생성될 때 할당
        self.action_space = gym.spaces.Discrete(2)
        self.observation_space = gym.spaces.Box(-180, 180, [1])

        print(self.observation_space.high)
        print(self.observation_space.low)

    def step(self, client, action):

        #전송할 배열
        array = np.array([self.id,action], dtype=np.float32)
        # 배열 크기 추가
        #array = np.insert(array, 0, len(array))
        # 배열 크기 및 내용 전송
        array_bytes = array.tobytes()
        client.sendall(array_bytes)

        state = []

        # wait till you get a response
        while (True):
            array_size_bytes = client.recv(4)
            if not array_size_bytes:
                break
            array_size = struct.unpack('!I', array_size_bytes)[0]  # 4바이트 정수 데이터를 해석하여 원래의 정수 값으로 언패킹하는 역할
            # 배열 데이터 수신
            array_data_bytes = client.recv(array_size * 4)
            array_data = np.frombuffer(array_data_bytes, dtype=np.float32)
            print("Step Array:", array_data)
            state = array_data.copy()
            break

        reward = 1

        done = False
        if (abs(state[0]) > 30):
            done = True
        elif (abs(state[1] > 800)):
            done = True


        return state, reward, done

    def reset(self, client):

        array = np.array([ self.id ,10], dtype=np.float32)
        # array = np.insert(array, 0, len(array))
        array_bytes = array.tobytes()
        client.sendall(array_bytes)

        state = [1,1,1,1]

        # wait till you get a response
        while (True):
            array_size_bytes = client.recv(4)
            if not array_size_bytes:
                break
            array_size = struct.unpack('!I', array_size_bytes)[0]  # 4바이트 정수 데이터를 해석하여 원래의 정수 값으로 언패킹하는 역할
            # 배열 데이터 수신
            array_data_bytes = client.recv(array_size * 4)
            array_data = np.frombuffer(array_data_bytes, dtype=np.float32)
            print("Reset Array:", array_data)
            state = array_data.copy()
            break

        state = [0,0,0,0]
        return state