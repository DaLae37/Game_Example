from multi_environment import multi_env
from PPO import PPO
import torch

from torch.utils.tensorboard import SummaryWriter
writer = SummaryWriter()

ENVCOUNT = 12
multi_env = multi_env(ENVCOUNT)
# ** new **
agent = PPO(input_dim=4, action_dim=2)

episode_history = [[] for x in range(ENVCOUNT)]
episode_rewards = [0 for x in range(ENVCOUNT)]
print(episode_history)
print(episode_rewards)

states = []

for r in range(ENVCOUNT):
    state = multi_env.reset(r)
    states.append(state)
    # print(f"id: {r}, state: {state}")

for timestep in range(100000): #timestep
    print(f"=== ep {timestep} ====")
    actions = []
    probs = []

    for id in range(ENVCOUNT):
        prob, action = agent.select_action(states[id])
        actions.append(action)
        probs.append(prob[action].item())

    # print(actions)
    n_states, rewards, dones = multi_env.step(actions)

    for i in range(ENVCOUNT):
        transition = (states[i], actions[i], rewards[i], n_states[i], probs[i], dones[i])
        # print(transition)
        agent.put_data((states[i], actions[i], rewards[i], n_states[i], probs[i], dones[i]))
        episode_rewards[i] += rewards[i]

        if(dones[i]):
            if(i==0):
                writer.add_scalar("Reward", episode_rewards[i], len(episode_history[i]))
            multi_env.reset(i)
            episode_history[i].append(episode_rewards[i])
            episode_rewards[i] = 0

    probs.clear()
    actions.clear()
    states.clear()

    states = n_states

    if timestep % 300 == 0 and timestep != 0:
        print("===trained===")
        for r in range(ENVCOUNT):
            state = multi_env.reset(r)
        agent.train_net()
        torch.save(agent.state_dict(), f"./save_weights/PPO.h5")