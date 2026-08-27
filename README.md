# Neural Network XOR -- C++

Un mic **neural network implementat de la zero în C++**, fără librării de Machine Learning.

Proiectul are ca scop înțelegerea modului în care funcționează matematic un neural network: **neuroni, weights, bias, forward propagation și backpropagation**.

Rețeaua este antrenată să învețe funcția **XOR**:

| Input 1 | Input 2 | Output |
| ------: | ------: | -----: |
|       0 |       0 |      0 |
|       0 |       1 |      1 |
|       1 |       0 |      1 |
|       1 |       1 |      0 |

## Arhitectura rețelei

Rețeaua folosită este:

```text
Input Layer       Hidden Layer       Output Layer

   x1 ─────┐       ○
            ├─────> ○ ─────┐
            │       ○       ├─────> ○
   x2 ─────┘       ○ ─────┘

   2 inputs        4 neurons          1 neuron
```

Adică o arhitectură **2 → 4 → 1**.

Fiecare conexiune dintre neuroni are un `weight`, iar fiecare neuron are propriul `bias`.

## Forward Propagation

Pentru fiecare neuron se calculează mai întâi:

$$
z = b + \sum_{i=1}^{n} x_iw_i
$$

unde:

* $x_i$ = input-ul
* $w_i$ = weight-ul asociat
* $b$ = bias-ul

Apoi valoarea este trecută prin funcția de activare **sigmoid**:

$$
\sigma(z)=\frac{1}{1+e^{-z}}
$$

În cod:

```cpp
double sigmoid(double x) {
    return 1.0/(1+exp(-x));
}
```

Rezultatul sigmoidului este între `0` și `1`, deci poate fi interpretat ca aproximarea rezultatului XOR.

## Backpropagation

După calcularea rezultatului, acesta este comparat cu valoarea corectă:

$$
error = target - output
$$

Pentru neuronul de output se calculează:

$$
\delta=(target-output)\cdot\sigma'(output)
$$

Derivata sigmoidului este:

$$
\sigma'(x)=x(1-x)
$$

Pentru neuronii din hidden layer, eroarea este propagată înapoi folosind weight-urile neuronilor următori.

În final, weight-urile sunt actualizate:

$$
w_{nou}=w_{vechi}+\eta\cdot\delta\cdot input
$$

iar bias-ul:

$$
b_{nou}=b_{vechi}+\eta\cdot\delta
$$

unde $\eta$ reprezintă **learning rate-ul**.

În proiect:

```cpp
double learning_rate = 0.5;
```

## Antrenare

Weight-urile și bias-urile sunt inițial generate aleatoriu, iar cele 4 cazuri XOR sunt repetate de mai multe ori.

După antrenare, programul permite introducerea a două valori și afișează predicția rețelei.

Exemplu:

```text
1 0
0.97
```

Valoarea nu trebuie să fie exact `1`, deoarece neural network-ul produce o **aproximare**.
