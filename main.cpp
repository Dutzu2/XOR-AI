#include <bits/stdc++.h>
/// Template Dutzu
#define fast ios_base::sync_with_stdio(false);cin.tie(0);
#define int long long
using namespace std;

ifstream fin("teste.in");
ofstream fout("teste.out");
class Neuron {
public:
    Neuron(const int &n);
    double bias,output,delta;
    double sum;
    vector<double> weights;
    int nr_weights;
    friend void genweights(Neuron &n);
    friend double random_weight();
protected:

private:
}; /// Clasa Neuron
class Layer {
public:
    Layer(const int n,const int nrInputsPerNeuron);
    int nr_neuroni;
    int nr_inputs;
    vector<Neuron> neuroni;
protected:

private:
}; /// Clasa Layer = Contine mai mult neuroni
double random_weight();
Neuron::Neuron(const int &n) {
    nr_weights=n;
    weights.resize(n);
    genweights(*this);
    bias=random_weight();
} /// Neuron constructor, are generarea de valori automata
Layer::Layer(const int n, const int inputs) {
    nr_neuroni=n;
    nr_inputs=inputs;
    for (int i=0; i<n; i++) {
        Neuron x(inputs);
        neuroni.push_back(x);
    }
} /// Layer constructor, creaza automat si neuronii din el
double sigmoid(double x) {
    return 1.0/(1+exp(-x));
} /// Functia de activare
double sigmoid_derivate(double x) {
    return x*(1-x);
} /// Functia de activare ^-1 pt backprop
double random_weight() {
    return ((double)rand()/RAND_MAX)*2-1;
} /// Functie de random
double random_bias() {
    return (double)rand()/RAND_MAX;
} /// Functie de random
void genweights(Neuron &n) {
    for (int i=0; i<n.nr_weights; i++)
        n.weights[i]=random_weight();
} /// Functie pt generarea de weights random
/// --------------------------------------------------------------------------------------------------- ///
vector<double> inputs; /// Inputs de la user
vector<Layer> layers; /// Hidden Layers + Output final
double learning_rate = 0.5; /// Pasul de learning (prea mic e prea lent, prea mare va duce la aproximare gresita)

void solve(int const nr_layer) {
    if (nr_layer==0) { /// Trebuie accesat din inputs
        for (auto neuron = layers[0].neuroni.begin(); neuron != layers[0].neuroni.end(); ++neuron) {
            neuron->sum = neuron -> bias;
            for (int i=0; i<inputs.size(); i++) {
                neuron->sum += inputs[i]*(neuron->weights[i]);
            }
            neuron->output = sigmoid(neuron->sum);
        }
    } else {
        for (auto neuron = layers[nr_layer].neuroni.begin(); neuron != layers[nr_layer].neuroni.end(); ++neuron) {
            neuron->sum = neuron -> bias;
            for (int i=0; i<layers[nr_layer-1].neuroni.size(); i++) {
                neuron->sum += layers[nr_layer-1].neuroni[i].output*(neuron->weights[i]);
            }
            neuron->output = sigmoid(neuron->sum);
        }
    }
} /// Functia principala de forward
void backprop(int nr_layer) {
    Layer &curentlayer=layers[nr_layer];
    Layer &nextlayer=layers[nr_layer+1];
    for (int i=0; i<curentlayer.nr_neuroni; i++) {
        Neuron &neuron = curentlayer.neuroni[i];
        double sum =0;

        for (int j=0; j<nextlayer.nr_neuroni; j++) {
            Neuron &nextneuron = nextlayer.neuroni[j];
            sum += nextneuron.weights[i]*nextneuron.delta;
        }
        neuron.delta = sigmoid_derivate(neuron.output)*sum;
    }

} /// Functia de backprop pentru delta
void recalweights(int nr_layer) {
    if (nr_layer==0) {
        Layer &layer=layers[nr_layer];
        for (int i=0; i<layer.nr_neuroni; i++) {
            Neuron &neuron = layer.neuroni[i];
            for (int j=0; j<neuron.nr_weights; j++)
                neuron.weights[j] += learning_rate * neuron.delta * inputs[j];
            neuron.bias+=learning_rate*neuron.delta;
        }
    } else {
        Layer &layer=layers[nr_layer];
        for (int i=0; i<layer.nr_neuroni; i++) {
            Neuron &neuron=layer.neuroni[i];
            for (int j=0; j<neuron.nr_weights; j++)
                neuron.weights[j] += learning_rate * neuron.delta * layers[nr_layer-1].neuroni[j].output;
            neuron.bias+=learning_rate*neuron.delta;
        }
    }

} /// Functia de backprop pentru valori
signed main() {
    ///fast
    srand(time(nullptr));
    int q;
    fin>>q;
    for (int i=0; i<1; i++) {
        Layer layer(4,2);
        layers.push_back(layer);
    }/// Hidden layers
    layers.push_back(Layer(1,4)); /// OUTPUT
    vector<tuple<int,int,int>> in;
    in.push_back({0,0,0});
    in.push_back({1,0,1});
    in.push_back({0,1,1});
    in.push_back({1,1,0});
    /**
    0 0 0
    1 0 1
    0 1 1
    1 1 0
    **/
    cout<<"AI-ul se antreneaza pe testele de baza: \n0^0=0\n1^0=1\n0^1=1\n1^1=0\nCu "<<q*4<<" teste\nLoading";
    for (int k=1;k<=q;k++){
    for (auto [a,b,target] : in){
    inputs.clear();
    inputs.push_back(a);
    inputs.push_back(b);
    for (int i=0; i<layers.size(); i++)
        solve(i); /// 1st Step
    double prediction,error;
    prediction = layers.back().neuroni[0].output;
    error = target - prediction;
    Neuron &out = layers.back().neuroni[0];
    out.delta=(target-out.output)*sigmoid_derivate(out.output); /// Verificarea rezultatului

    for (int i=layers.size()-2; i>=0; i--)
        backprop(i); /// Propagarea erorii inapoi

    for (int i=layers.size()-1; i>=0; i--)
        recalweights(i); /// Recalcularea dupa verificarea erorii

    for (int i=0; i<layers.size(); i++)
        solve(i); /// Recalculare forward
    if (k%(q/100)==0);
        ///fout<<layers.back().neuroni[0].output<<'\n';
        }
    if (k%(q/10)==0)
        cout<<".";
    }
    /// AI-ul a fost antrenat pentru XOR 1-0
    cout<<'\n';
    int a,b;
    cout<<"Puteti pune numere pentru calcularea XOR:\n";
    int n;
    while(true){
    cin>>a>>b;
    inputs.clear();
    inputs.push_back(a);
    inputs.push_back(b);
    for (int i=0; i<layers.size(); i++)
        solve(i);
    cout<<layers.back().neuroni[0].output<<'\n';
    getchar();
    }
    return 0;
}
