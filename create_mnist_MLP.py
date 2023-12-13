import tensorflow as tf
import numpy as np
import pickle
from tensorflow_model_optimization.python.core.quantization.keras import quantize

# Load and preprocess the MNIST dataset
mnist = tf.keras.datasets.mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
train_images, test_images = train_images, test_images

# Manually flatten the images
train_images_flat = train_images.reshape((train_images.shape[0], -1))
test_images_flat = test_images.reshape((test_images.shape[0], -1))

# Define a simple neural network model
model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(10, activation='softmax'),
])

# Compile the model
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

# Train the model on the MNIST dataset
model.fit(train_images_flat, train_labels, epochs=5)

# Evaluate the model on the test set
test_loss, test_acc = model.evaluate(test_images_flat, test_labels)
print(f"\nTest accuracy: {test_acc}")

# Save the trained model to a .pkl file
model_weights = {}
for layer in model.layers:
    layer_name = layer.name
    layer_weights = layer.get_weights()
    model_weights[layer_name] = layer_weights

# Save model weights to a .pkl file
with open('mnist_model_weightst.pkl', 'wb') as file:
    pickle.dump(model_weights, file)

quantized_model_weights = {}
for layer in model.layers:
    layer_name = layer.name
    layer_weights = layer.get_weights()
    # Quantize weights using TensorFlow's quantization API
    quantized_weights = [((w-np.min(w))*255/(np.max(w)-np.min(w))).astype(np.uint8) for w in layer_weights]
    quantized_model_weights[layer_name] = quantized_weights


# Save quantized model weights to a .pkl file
with open('mnist_quantized_model_weights.pkl', 'wb') as file:
    pickle.dump(quantized_model_weights, file)

model2 = tf.keras.models.Sequential([
    tf.keras.layers.Dense(10, activation='softmax'),
])
model2.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])
model2.fit(train_images_flat, train_labels, epochs=1)

for layer in model2.layers:
  layer.set_weights(quantized_weights)

test_loss, test_acc = model2.evaluate(test_images_flat, test_labels)
print(f"\nTest accuracy Quantized: {test_acc}")

with open("weight_file.txt","w") as wf:
  for i in quantized_model_weights:
      weights = quantized_model_weights[i][0]
      biases = quantized_model_weights[i][1]
      wf.write("Weights:\n")
      for i in weights:
        i = [str(j) for j in i]
        wf.write("\t".join(i))
        wf.write("\n")
      wf.write("Biases:\n")
      biases = [str(j) for j in biases]
      wf.write("\t".join(biases))

with open("input_file.txt","w") as inf:
    for i in range(len(test_images_flat)):
        test_input = test_images_flat[i]
        test_input = [str(i) for i in test_input]
        inf.write("Input:\n")
        inf.write(" ".join(test_input))
        inf.write("\n")
        inf.write("Label:"+str(test_labels[i])+"\n")
    inf.write("END\n")