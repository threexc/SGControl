import numpy as np
import matplotlib.pyplot as plt

modulator_frequency = 10.0
carrier_frequency = 40.0
modulation_index = 1.0

test_string = "01110100 01100101 01110011 01110100 00100000 01101101 01100101 \
01110011 01110011 01100001 01100111 01100101"
test_string = test_string.replace(" ", "")
data_map = map(int, test_string)
data_array = np.fromiter(data_map, int)
zero_pad = np.zeros(40)
data_array = np.append(data_array, zero_pad)
data_array = np.tile(data_array, 4)

print(len(data_array))
time = np.arange(544) / 544
carrier = np.sin(2.0 * np.pi * carrier_frequency * time)
product = np.zeros_like(time)
randomizer_signal = np.random.rand(len(product))
modulator_signal = np.zeros_like(time)


for i, t in enumerate(time):
    if data_array[i] == 1:
        product[i] = np.sin(2. * np.pi * ((carrier_frequency) * t))
    else:
        product[i] = 0

plt.subplot(3, 1, 1)
plt.title('Amplitude-Shift Keying')
plt.plot(data_array, color = 'black', drawstyle = 'steps-pre')
plt.ylabel('Amplitude')
plt.xlabel('Modulator signal')
plt.subplot(3, 1, 2)
plt.plot(carrier)
plt.ylabel('Amplitude')
plt.xlabel('Carrier signal')
plt.subplot(3, 1, 3)
plt.plot(product)
plt.ylabel('Amplitude')
plt.xlabel('Output signal')
plt.show()
