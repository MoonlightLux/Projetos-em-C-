#include <iostream>
#include <vector>
#include <string>
#include <fstream> //para manipular o arquivo
#include <cstdlib> //para a limparTela()
#include <iomanip> //pra formatar pra duas casas decimais

using namespace std;

struct Aluno{
	string matricula;
	string nome;
};

struct Nota {
    string matriculaAluno;
    string tipoNota; //tipo da nota/codigo da prova: P1, P2, trabalho, etc...
    double valor;
    double peso;
};

vector<Aluno> alunos;
vector<Nota> notas;

void carregarAlunos(){	
	ifstream arquivo("aluno.csv"); //abrir o arquivo
	
	if(!arquivo.is_open()){ //criar o arquivo se ele nao existir
		ofstream novoArquivo("aluno.csv");
		
		if(novoArquivo.is_open()){
			novoArquivo << "matricula;nome" << endl;
			novoArquivo.close();
		}
		
		return;
		
	}
	
	string linha;
	
	getline(arquivo, linha); //pula a primeira linha q eh o cabecalho
	
	while(getline(arquivo, linha)){
		if(linha.empty()){ //pular se a linha tiver vazia
			continue;
		}
		
		Aluno aluno;
		string temp = linha; //string temporaria pra nao mexer na linha real
		
		size_t pos = temp.find(';'); //acha, salva e apaga a matricula pra ver o proximo conteudo
		aluno.matricula = temp.substr(0, pos);
		temp.erase(0, pos +1);
		
		pos = temp.find(';');
		aluno.nome = temp.substr(0, pos);
		temp.erase(0, pos + 1);
		
		alunos.push_back(aluno);
	}
	
	arquivo.close();
	
}

/*
ifstream = ler arquivos
ofstream = escrever nos arquivos
getline(arquivo, linha) = ler cada linha do arquivo
size_t = tipo int, mas pra tamanhos
stoi() = converte string pra int
string.substr(posicao, tamanho) = extrai uma parte da string
*/

vector<string> split(string texto, char separador) {
    vector<string> partes;
    string atual;
    for (char c : texto) {
        if (c == separador) {
            partes.push_back(atual);
            atual.clear();
        } else {
            atual += c;
        }
    }
    partes.push_back(atual);
    return partes;
}

void salvarAlunos(){
	ofstream arquivo("aluno.csv");
	
	if(!arquivo.is_open()){
		cout << "Erro ao salvar o arquivo." << endl;
		return;
	}
	
	arquivo << "matricula;nome" << endl;
	
	for(const auto& aluno : alunos){
		arquivo << aluno.matricula << ";" << aluno.nome << endl;
	}
	
	arquivo.close();
	
}

//const auto& = usado pra nao criar copias, nao sobrescrever, pra ser mais seguro no geral

void cadastrarAluno(){
	Aluno novoAluno;
	
	do{
		cout << "Matricula: ";
		getline(cin, novoAluno.matricula);
		
		if(novoAluno.matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(novoAluno.matricula.empty());
	
	
	for(const auto& aluno : alunos){
		if(aluno.matricula == novoAluno.matricula){
			cout << "Erro: matricula ja existe." << endl;
			return;
		}
	}
	
	do{
		cout << "Nome: ";
		getline(cin, novoAluno.nome);
		
		if(novoAluno.nome.empty()){
			cout << "Erro: nome nao pode ficar em branco." << endl;
		}
	}while(novoAluno.nome.empty());
	
	alunos.push_back(novoAluno);
	salvarAlunos();
	
	cout << "Aluno cadastrado." << endl;
	
}

void printarAluno(const Aluno& aluno) {
	cout << "Matricula: " << aluno.matricula << endl;
    cout << "Nome: " << aluno.nome << endl;
    cout << "-------------------" << endl;
}

void listarAlunos(){
	if(alunos.empty()){
	cout << "Nenhum aluno cadastrado." << endl;
	return;	
	}
	
	cout << "Lista de alunos:" << endl;
	
	for(const auto& aluno : alunos){
		printarAluno(aluno);
	}
	
}

bool matriculaExiste(string matricula){ //util pra checar se a matricula existe antes de emprestar
	for(const auto& aluno : alunos){
		if(aluno.matricula == matricula){
			return true;
		}	
	}
	
	return false;
	
}

void limparTela(){ //util pro menu de vdd
	#ifdef _WIN32
		system("cls");
	
	#else
		system("clear");
	
	#endif
}

//ifdef decide na compilacao, nao na execucao

bool alunoTemNotas(string matricula){ //funcao pra listarAlunoEspec ficar menos confusa
	for(const auto& nota : notas){
		if(nota.matriculaAluno == matricula){
			return true; //encontrou pelo menos uma nota
		}
	}
	
	return false; //nao encontrou nenhuma nota
}

void apagarAluno(){
	string matricula;
	
	do{
		cout << "Digite a matricula do aluno a ser apagado:";
		getline(cin, matricula);
		
		if(matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(matricula.empty());
	
	if(alunoTemNotas(matricula)){
		cout << "Erro: voce nao pode apagar um aluno que possui notas cadastradas." << endl;
		return;
	}
	
	for (auto it = alunos.begin(); it != alunos.end(); ++it){
		if(it -> matricula == matricula){
			cout << "Aluno " << it -> nome << " (" << it->matricula << ") apagado" << endl;
			alunos.erase(it);
			salvarAlunos();
			return;
		}
	}
	
	cout << "Aluno nao encontrado." << endl;
}

void editarAluno(){
	string matricula;
	string novoNome;
	
	do{
		cout << "Digite a matricula do aluno a ser editado:";
		getline(cin, matricula);
		
		if(matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(matricula.empty());
	 
	for(auto& aluno : alunos){
		if(aluno.matricula == matricula){
		 	cout << endl << "/// Editando Aluno ///" << endl << "Dados atuais:" << endl;
		 	printarAluno(aluno);
		 	
			do{
				cout << "Insira o novo nome: ";
				getline(cin, novoNome);
				
				if(novoNome.empty()){
					cout << "Erro: nome nao pode ficar em branco." << endl;
				}
			}while(novoNome.empty());
			
		 	aluno.nome = novoNome;
			salvarAlunos();
			cout << "Aluno editado com sucesso." << endl;
			return;
		}
	}
	 cout << "Aluno nao encontrado." << endl;
}

void carregarNotas(){
	ifstream arquivo("notas.csv");
	
	if(!arquivo.is_open()){
		ofstream novoArquivo("notas.csv");
		
		if(novoArquivo.is_open()){
			novoArquivo << "matricula_aluno;tipoNota;valor;peso" << endl;
			novoArquivo.close();
		}
		return;
	}
	
	string linha;
	getline(arquivo, linha);
	
	while(getline(arquivo, linha)){
		if(linha.empty()){
			continue;
		}
		
		Nota nota; //nome meio confuso, mas eh uma nota da classe Nota
		vector<string> campos = split(linha, ';'); //dividir em campos diferentes pra formatacao
		
		if(campos.size() >= 4){
			nota.matriculaAluno = campos[0];
			nota.tipoNota = campos[1];
			try{
				nota.valor = stod(campos[2]);
				nota.peso = stod(campos[3]);
			}
			
			catch(...){
				nota.valor = 0.0;
				nota.peso = 1.0;
			}
			
			notas.push_back(nota);
		}
		
	}
	arquivo.close();
}
/*
stod() transforma uma string em um double
try - catch(...) = tenta isso, se nao der, ignore e faca aquilo
catch tem outros argumentos que pode usar alem do (...), mas eu nao entendi :)
*/

void salvarNotas(){
	ofstream arquivo("notas.csv");
	if(!arquivo.is_open()){
		cout << "Erro ao salvar o arquivo de notas." << endl;
		return;
	}
	
	arquivo << "matricula_aluno;tipoNota;valor;peso" << endl; //ofstream sobrescreve, entao precisa colocar de novo
	
	for (const auto& nota : notas){ //agora tah formatando do jeito certo no csv tbm
		arquivo << nota.matriculaAluno << ";" << nota.tipoNota << ";" << fixed << setprecision(2) << nota.valor << ";" << fixed << setprecision(2) << nota.peso << endl;
	}
	
	arquivo.close();
}

bool notaExiste(string matricula, string tipoNota){
	for(const auto& nota : notas){
		if(nota.matriculaAluno == matricula && nota.tipoNota == tipoNota){
			return true;
		}
	}
	
	return false;
}

double lerDouble(string texto){ //tava repetindo muito codigo sem isso
	string input;
	double valor;
	
	while(true){
		cout << texto;
		getline(cin, input);
		
		try{
			valor = stod(input);
			return valor;
		}
		
		catch(...){
			cout << "Erro: valor invalido. Tente novamente." << endl;
		}
	}
	
}

void cadastrarNota(){
	string matricula, tipoNota, valorStr, pesoStr;
	double valor, peso;
	
	do{
		cout << "Matricula do aluno: ";
		getline(cin, matricula);
		
		if(matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(matricula.empty());
	
	if(!matriculaExiste(matricula)){
		cout << "Erro, aluno nao encontrado" << endl;
		return;
	}
	
	do{
		cout << "Tipo da nota (P1 para Prova 1, T1 para Trabalho 1, P2, T2, etc.): ";
		getline(cin, tipoNota);
		
		if(tipoNota.empty()){
			cout << "Erro: tipo da nota nao pode ficar em branco." << endl;
		}
	}while(tipoNota.empty());
	
	if(notaExiste(matricula, tipoNota)){
		cout << "Erro: ja existe uma nota desse tipo para esse aluno." << endl;
		return;
	}
	
	valor = lerDouble("Valor da nota: ");
	peso = lerDouble("Peso da nota: ");
	
	Nota novaNota; //cria e salva a nova nota
	novaNota.matriculaAluno = matricula;
	novaNota.tipoNota = tipoNota;
	novaNota.valor = valor;
	novaNota.peso = peso;
	
	notas.push_back(novaNota);
	salvarNotas();
	
	cout << "Nota cadastrada com sucesso." << endl;
	
}

void editarNota(){
	string matricula, tipoNota;
	
	do{
		cout << "Matricula do aluno: ";
		getline(cin, matricula);
		
		if(matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(matricula.empty());
	
	if(!matriculaExiste(matricula)){
		cout << "Erro: aluno nao existe ou nao encontrado." << endl;
		return;
	}
	
	do{
		cout << "Tipo da nota a editar (P1, P2, T1, T2, etc.): ";
		getline(cin, tipoNota);
		
		if(tipoNota.empty()){
			cout << "Erro: tipo da nota nao pode ficar em branco." << endl;
		}
	}while(tipoNota.empty()); //depois de repetir isso tantas vezes, vejo que uma funcao que impede strings vazias seria melhor
	
	for(auto& nota : notas){
		if(nota.matriculaAluno == matricula &&  nota.tipoNota == tipoNota){
			cout << endl << "/// Editando Nota ///" << endl;;
			cout << "Dados atuais: " << endl;
			cout << "Valor: " << nota.valor << endl;
			cout << "Peso: " << nota.peso << endl;
			cout << "-------------------" << endl; //nao sei se ficou simetrico como o de aluno :')
			
			nota.valor = lerDouble("Novo valor: ");
			nota.peso = lerDouble("Novo peso: ");
			
			salvarNotas();
			cout << "Nota editada com sucesso." << endl;
			return;
		}
	}
	
	cout << "Erro: nota nao encontrada." << endl;
}

double calcularMedia(string matricula){
	double somaNotas = 0;
	double somaPesos = 0;
	
	for(const auto& nota : notas){
		if(nota.matriculaAluno == matricula){
			somaNotas += nota.valor * nota.peso;
			somaPesos += nota.peso;
		}
	}
	
	if(somaPesos == 0){
		return 0.0; //retorna 0 se nao tiver notas
	}
	
	return somaNotas / somaPesos;
	
}

void listarAlunoEspec(){ //listar um aluno especifico
	string matricula;
	
	do{
		cout << "Matricula aluno: ";
		getline(cin, matricula);
		
		if(matricula.empty()){
			cout << "Erro: matricula nao pode ficar em branco." << endl; //eu realmente deveria ter feito uma funcao pra isso
		}
	}while(matricula.empty());
	
	if(!matriculaExiste(matricula)){
		cout << "Aluno nao encontrado." << endl;
		return;
	}
	
	Aluno alunoEncontrado;
	for(auto& aluno : alunos){
		if(aluno.matricula == matricula){
			alunoEncontrado = aluno;
			break;
		}
	}
	
	cout << endl << "/// Dados do aluno ///" << endl; //nao sei se ficou simetrico tbm
	printarAluno(alunoEncontrado);
	
	if(!alunoTemNotas(matricula)){
		cout << "Nenhuma nota cadastrada." << endl;
	}
	
	else{
		cout << "Notas: " << endl;
		for(const auto& nota : notas){
			if(nota.matriculaAluno == matricula){
				cout << "- " << nota.tipoNota << ": " << fixed << setprecision(2) << nota.valor << " (peso: " << fixed << setprecision(2) << nota.peso << ")" << endl;

			}
		}
	}
	
	double media = calcularMedia(matricula);
	cout << "Media: " << fixed << setprecision(2) << media << endl;
	cout << "----------------------------------------" << endl;
	
}

void apagarNota(){
    string matricula, tipoNota;
    
    do{
    	cout << "Matricula do aluno: ";
    	getline(cin, matricula);
    	
    	if(matricula.empty()){
    		cout << "Erro: matricula nao pode ficar em branco." << endl;
		}
	}while(matricula.empty());
	
	if(!matriculaExiste(matricula)){
		cout << "Erro: aluno nao existe ou nao encontrado." << endl;
		return;
	}
    
    do{
    	cout << "Tipo da nota a apagar (P1, P2, T1, etc.): ";
    	getline(cin, tipoNota);
    	
    	if(tipoNota.empty()){
    		cout << "Erro: o tipo da nota nao pode ficar em branco" << endl;
		}
	}while(tipoNota.empty());
    
    //usado pra iterar em todo o vetor notas com a posicao especifica do lugar a ser apagado    
    for(auto it = notas.begin(); it != notas.end(); ++it){
        if(it->matriculaAluno == matricula && it->tipoNota == tipoNota){
            cout << "Nota " << it->tipoNota << " do aluno " << it->matriculaAluno << " apagada." << endl;
            notas.erase(it);
            salvarNotas();
            return;
        }
    }
    
    cout << "Erro: nota nao encontrada." << endl;
}

//-> eh usado para ponteiros assim como o . eh usado em um objeto pra acessar aquele atributo

void listarAlunosNotas(){
    if(alunos.empty()){
        cout << "Nenhum aluno cadastrado." << endl;
        return;
    }
    
    cout << endl << "/// Todos os alunos com suas respectivas notas e medias: ///" << endl; //ficou meio feio :/
    
    for(const auto& aluno : alunos){
        cout << endl << "/// Aluno: " << aluno.nome << " ///" << endl << "Matricula: " << aluno.matricula << endl;
        cout << "----------------------------------------" << endl; //ficou bom, nao precisa mexer mais
        
        if(!alunoTemNotas(aluno.matricula)){
            cout << "Nenhuma nota cadastrada." << endl;
            cout << "Media: 0.0" << endl;
        }
        else{
            cout << "Notas: " << endl << endl;
            for(const auto& nota : notas){
                if(nota.matriculaAluno == aluno.matricula){
                    cout << "- " << nota.tipoNota << ": " << fixed << setprecision(2) << nota.valor << " (peso: " << fixed << setprecision(2) << nota.peso << ")" << endl;
                }
            }
            
            double media = calcularMedia(aluno.matricula);
            cout << endl << "Media: " << fixed << setprecision(2) << media << endl;
        }
        cout << "----------------------------------------" << endl;
    }
}
void menuTeste(){
	carregarAlunos();
	carregarNotas();
	int opcao;
	string matricula; //nao tava funcionando declarando dentro do case (???)
	
	do{
		cout << "1 - Cadastrar aluno;" << endl;
		cout << "2 - Apagar aluno;" << endl;
		cout << "3 - Editar aluno;" << endl;
		cout << "4 - Listar todos os alunos;" << endl;
		cout << "5 - Cadastrar nota;" << endl;
		cout << "6 - Apagar nota;" << endl;
		cout << "7 - Editar nota;" << endl;
		cout << "8 - Listar nota(s) de um aluno;" << endl;
		cout << "9 - Listar todos os alunos com suas respectivas notas e medias;" << endl;
		cout << "0 - Sair." << endl;
		cout << "Digite sua opcao: ";
		cin >> opcao;
		cin.ignore();
		
		switch(opcao){
			case 1:
				limparTela();
				cadastrarAluno();
				break;
			
			case 2:
				limparTela();
				apagarAluno();
				break;
			
			case 3:
				limparTela();
				editarAluno();
				break;

			
			case 4:
				limparTela();
				listarAlunos();
				break;
				
			case 5:
				limparTela();
				cadastrarNota();
				break;
				
			case 6:
				limparTela();
				apagarNota();
				break;
				
			case 7:
				limparTela();
				editarNota();
				break;
				
			case 8:
				limparTela();
				listarAlunoEspec();
				break;
				
			case 9:
				limparTela();
				listarAlunosNotas();
				break;
				
			case 0:
				cout << "Saindo.";
				exit(0);
				
			default:
				cout << "Opcao invalida." << endl;
		}
		
		if(opcao != 0){ //soh pra deixar o prompt mais bonitinho
			cout << endl << "Pressione enter pra continuar";
			cin.get(); //espera o usuario digitar enter
		}
		
		limparTela();
		
	} while(opcao != 0);
	
}

int main(){
	menuTeste();
	
	return 0;
}
