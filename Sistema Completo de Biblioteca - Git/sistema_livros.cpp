#include <iostream>
#include <vector>
#include <string>
#include <fstream> //para manipular o arquivo
#include <cstdlib> //para a limparTela()
#include <sstream>
#include <iomanip>

using namespace std;

struct Aluno{
	string matricula;
	string nome;
	string curso;
	string email;
};

struct Livro {
    string codigo;
    string titulo;
    string autor;
    int ano_publicacao;
};

struct Emprestimo {
    int codigo_emprestimo;
    string matricula_aluno;
    string codigo_livro;
    string data_emprestimo;
    string data_devolucao;
    string status; // "Aberto" ou "Devolvido"
};

vector<Aluno> alunos;
vector<Livro> livros;
vector<Emprestimo> emprestimos;

void carregarAlunos(){	
	ifstream arquivo("aluno.csv"); //abrir o arquivo
	
	if(!arquivo.is_open()){ //criar o arquivo se ele nao existir
		ofstream novoArquivo("aluno.csv");
		
		if(novoArquivo.is_open()){
			novoArquivo << "matricula;nome;curso;email" << endl;
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
		
		pos = temp.find(';');
		aluno.curso = temp.substr(0, pos);
		temp.erase(0, pos + 1);
		
		aluno.email = temp;
		
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

string obterDataAtual() {
    time_t agora = time(0);
    tm *ltm = localtime(&agora);
    ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-" 
        << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
        << setfill('0') << setw(2) << ltm->tm_mday;
    return oss.str();
}

void salvarAlunos(){
	ofstream arquivo("aluno.csv");
	
	if(!arquivo.is_open()){
		cout << "Erro ao salvar o arquivo." << endl;
		return;
	}
	
	arquivo << "matricula;nome;curso;email" << endl;
	
	for(const auto& aluno : alunos){
		arquivo << aluno.matricula << ";" << aluno.nome << ";" << aluno.curso << ";" << aluno.email << endl;
	}
	
	arquivo.close();
	
}

//const auto& = usado pra nao criar copias, nao sobrescrever, pra ser mais seguro no geral

void cadastrarAluno(){
	Aluno novoAluno;
	
	cout << "Matricula: ";
	getline(cin, novoAluno.matricula);
	
	
	for(const auto& aluno : alunos){
		if(aluno.matricula == novoAluno.matricula){
			cout << "Erro: matricula ja existe" << endl;
			return;
		}
	}
	
	cout << "Nome: ";
	getline(cin, novoAluno.nome);
	
	cout << "Curso: ";
	getline(cin, novoAluno.curso);
	
	cout << "Email: ";
	getline(cin, novoAluno.email);
	
	alunos.push_back(novoAluno);
	salvarAlunos();
	
	cout << "Aluno cadastrado." << endl;
	
}

/*
try - catch(...) = tenta isso, se n�o der, ignore e faca aquilo
catch tem outros argumentos que pode usar alem do (...), mas eu nao entendi :)
*/

void printarAluno(const Aluno& aluno) {
	cout << "Matricula: " << aluno.matricula << endl;
    cout << "Nome: " << aluno.nome << endl;
    cout << "Curso: " << aluno.curso << endl;
    cout << "Email: " << aluno.email << endl;
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

bool alunoEstaEmprestando(string matricula) {
    for (const auto& emp : emprestimos) {
        if (emp.matricula_aluno == matricula && emp.status == "Aberto") {
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

void apagarAluno(){
	string matricula;
	cout << "Digite a matricula do aluno a ser apagado:";
	getline(cin, matricula);
	
    if (alunoEstaEmprestando(matricula)) {
        cout << "Erro: nao e possivel apagar um aluno que esta emprestando um livro!" << endl;
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
	cout << "Digite a matricula do aluno a ser editado:";
	getline(cin, matricula);
	 
	for(auto& aluno : alunos){
		if(aluno.matricula == matricula){
		 	cout << endl << "--- Editando Aluno ---" << endl << "Dados atuais:" << endl;
		 	printarAluno(aluno);
		 	
		 	cout << "Insira o novo nome: ";
				getline(cin, novoNome);
		 		aluno.nome = novoNome;
		 	
			salvarAlunos();
			cout << "Aluno editado com sucesso." << endl;
			return;
		}
	}
	 cout << "Aluno nao encontrado." << endl;
}

// ==================== FUNÇÕES DE LIVROS ====================
void carregarLivros() {
    ifstream arquivo("livro.csv");
    
    if (!arquivo.is_open()) {
        ofstream novoArquivo("livro.csv");
        if (novoArquivo.is_open()) {
            novoArquivo << "codigo;titulo;autor;ano_publicacao" << endl;
            novoArquivo.close();
        }
        return;
    }
    
    string linha;
    getline(arquivo, linha); // Pula cabeçalho
    
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        Livro livro;
        vector<string> campos = split(linha, ';');
        
        if (campos.size() >= 4) {
            livro.codigo = campos[0];
            livro.titulo = campos[1];
            livro.autor = campos[2];
            try {
                livro.ano_publicacao = stoi(campos[3]);
            } catch (...) {
                livro.ano_publicacao = 0;
            }
            livros.push_back(livro);
        }
    }
    
    arquivo.close();
}

void salvarLivros() {
    ofstream arquivo("livro.csv");
    
    if (!arquivo.is_open()) {
        cout << "Erro ao salvar o arquivo de livros." << endl;
        return;
    }
    
    arquivo << "codigo;titulo;autor;ano_publicacao" << endl;
    
    for (const auto& livro : livros) {
        arquivo << livro.codigo << ";" << livro.titulo << ";" 
                << livro.autor << ";" << livro.ano_publicacao << endl;
    }
    
    arquivo.close();
}

bool codigoLivroExiste(string codigo) {
    for (const auto& livro : livros) {
        if (livro.codigo == codigo) {
            return true;
        }
    }
    return false;
}

bool livroEstaEmprestado(string codigo) {
    for (const auto& emp : emprestimos) {
        if (emp.codigo_livro == codigo && emp.status == "Aberto") {
            return true;
        }
    }
    return false;
}

void cadastrarLivro() {
    Livro novoLivro;
    
    cout << "Codigo (ISBN): ";
    getline(cin, novoLivro.codigo);
    
    for (const auto& livro : livros) {
        if (livro.codigo == novoLivro.codigo) {
            cout << "Erro: codigo ja existe!" << endl;
            return;
        }
    }
    
    cout << "Titulo: ";
    getline(cin, novoLivro.titulo);
    
    cout << "Autor: ";
    getline(cin, novoLivro.autor);
    
    cout << "Ano de Publicacao: ";
    string ano_str;
    getline(cin, ano_str);
    
    try {
        novoLivro.ano_publicacao = stoi(ano_str);
    } catch (...) {
        cout << "Ano invalido. Usando 0." << endl;
        novoLivro.ano_publicacao = 0;
    }
    
    livros.push_back(novoLivro);
    salvarLivros();
    
    cout << "\n- Livro cadastrado com sucesso!" << endl;
}

void apagarLivro() {
    string codigo;
    cout << "Digite o codigo do livro a ser apagado: ";
    getline(cin, codigo);
    
    if (livroEstaEmprestado(codigo)) {
        cout << "Erro: nao e possivel apagar um livro que esta emprestado!" << endl;
        return;
    }
    
    for (auto it = livros.begin(); it != livros.end(); ++it) {
        if (it->codigo == codigo) {
            cout << "Livro \"" << it->titulo << "\" (Codigo: " << it->codigo 
                 << ") apagado com sucesso!" << endl;
            livros.erase(it);
            salvarLivros();
            return;
        }
    }
    
    cout << "Livro nao encontrado." << endl;
}

void printarLivro(const Livro& livro) {
	cout << "\nCodigo: " << livro.codigo << endl;
    cout << "Titulo: " << livro.titulo << endl;
    cout << "Autor: " << livro.autor << endl;
    cout << "Ano: " << livro.ano_publicacao << endl;
    cout << "-------------------" << endl;
}

void editarLivro(){
	string codigo;
	string novoTitulo;
	cout << "Digite o codigo do livro a ser editado:";
	getline(cin, codigo);
	 
	for(auto& livro : livros){
		if(livro.codigo == codigo){
		 	cout << endl << "--- Editando Livro ---" << endl << "Dados atuais:" << endl;
		 	printarLivro(livro);
		 	
		 	cout << "Insira o novo titulo: ";
				getline(cin, novoTitulo);
		 		livro.titulo = novoTitulo;
		 	
			salvarLivros();
			cout << "Livro editado com sucesso." << endl;
			return;
		}
	}
	 cout << "Livro nao encontrado." << endl;
}

void listarLivros() {
    if (livros.empty()) {
        cout << "Nenhum livro cadastrado." << endl;
        return;
    }
    
    cout << "\n=== LISTA DE LIVROS ===" << endl;
    for (const auto& livro : livros) {
        cout << "\nCodigo: " << livro.codigo << endl;
        cout << "Titulo: " << livro.titulo << endl;
        cout << "Autor: " << livro.autor << endl;
        cout << "Ano: " << livro.ano_publicacao << endl;
        
        if (livroEstaEmprestado(livro.codigo)) {
            cout << "Status: EMPRESTADO" << endl;
        } else {
            cout << "Status: DISPONIVEL" << endl;
        }
        cout << "-------------------" << endl;
    }
}

// ==================== FUNÇÕES DE EMPRÉSTIMOS ====================
void carregarEmprestimos() {
    ifstream arquivo("emprestimo.csv");
    
    if (!arquivo.is_open()) {
        ofstream novoArquivo("emprestimo.csv");
        if (novoArquivo.is_open()) {
            novoArquivo << "codigo_emprestimo;matricula_aluno;codigo_livro;data_emprestimo;data_devolucao;status" << endl;
            novoArquivo.close();
        }
        return;
    }
    
    string linha;
    getline(arquivo, linha); // Pula cabeçalho
    
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        Emprestimo emp;
        vector<string> campos = split(linha, ';');
        
        if (campos.size() >= 6) {
            try {
                emp.codigo_emprestimo = stoi(campos[0]);
            } catch (...) {
                emp.codigo_emprestimo = 0;
            }
            emp.matricula_aluno = campos[1];
            emp.codigo_livro = campos[2];
            emp.data_emprestimo = campos[3];
            emp.data_devolucao = campos[4];
            emp.status = campos[5];
            emprestimos.push_back(emp);
        }
    }
    
    arquivo.close();
}

void salvarEmprestimos() {
    ofstream arquivo("emprestimo.csv");
    
    if (!arquivo.is_open()) {
        cout << "Erro ao salvar o arquivo de emprestimos." << endl;
        return;
    }
    
    arquivo << "codigo_emprestimo;matricula_aluno;codigo_livro;data_emprestimo;data_devolucao;status" << endl;
    
    for (const auto& emp : emprestimos) {
        arquivo << emp.codigo_emprestimo << ";" << emp.matricula_aluno << ";" 
                << emp.codigo_livro << ";" << emp.data_emprestimo << ";"
                << emp.data_devolucao << ";" << emp.status << endl;
    }
    
    arquivo.close();
}

int gerarCodigoEmprestimo() {
    int maior = 0;
    for (const auto& emp : emprestimos) {
        if (emp.codigo_emprestimo > maior) {
            maior = emp.codigo_emprestimo;
        }
    }
    return maior + 1;
}

void registrarEmprestimo() {
    string matricula, codigo;
    
    cout << "Matricula do aluno: ";
    getline(cin, matricula);
    
    if (!matriculaExiste(matricula)) {
        cout << "Erro: aluno nao encontrado!" << endl;
        return;
    }
    
    cout << "Codigo do livro: ";
    getline(cin, codigo);
    
    if (!codigoLivroExiste(codigo)) {
        cout << "Erro: livro nao encontrado!" << endl;
        return;
    }
    
    if (livroEstaEmprestado(codigo)) {
        cout << "Erro: livro ja esta emprestado!" << endl;
        return;
    }
    
    Emprestimo novoEmp;
    novoEmp.codigo_emprestimo = gerarCodigoEmprestimo();
    novoEmp.matricula_aluno = matricula;
    novoEmp.codigo_livro = codigo;
    novoEmp.data_emprestimo = obterDataAtual();
    novoEmp.data_devolucao = "";
    novoEmp.status = "Aberto";
    
    emprestimos.push_back(novoEmp);
    salvarEmprestimos();
    
    cout << "\n- Emprestimo registrado com sucesso!" << endl;
    cout << "Codigo do emprestimo: " << novoEmp.codigo_emprestimo << endl;
    cout << "Data do emprestimo: " << novoEmp.data_emprestimo << endl;
}

void registrarDevolucao() {
    string codigo;
    cout << "Codigo do livro a ser devolvido: ";
    getline(cin, codigo);
    
    for (auto& emp : emprestimos) {
        if (emp.codigo_livro == codigo && emp.status == "Aberto") {
            emp.data_devolucao = obterDataAtual();
            emp.status = "Devolvido";
            salvarEmprestimos();
            
            cout << "\n- Devolucao registrada com sucesso!" << endl;
            cout << "Data da devolucao: " << emp.data_devolucao << endl;
            return;
        }
    }
    
    cout << "Erro: nenhum emprestimo em aberto encontrado para este livro!" << endl;
}

void listarEmprestimos() {
    if (emprestimos.empty()) {
        cout << "Nenhum emprestimo registrado." << endl;
        return;
    }
    
    cout << "\n=== LISTA DE EMPRESTIMOS ===" << endl;
    
    for (const auto& emp : emprestimos) {
        cout << "\nCodigo Emprestimo: " << emp.codigo_emprestimo << endl;
        cout << "Matricula Aluno: " << emp.matricula_aluno << endl;
        cout << "Codigo Livro: " << emp.codigo_livro << endl;
        cout << "Data Emprestimo: " << emp.data_emprestimo << endl;
        cout << "Data Devolucao: " << (emp.data_devolucao.empty() ? "Pendente" : emp.data_devolucao) << endl;
        cout << "Status: " << emp.status << endl;
        cout << "-------------------" << endl;
    }
}

void menuTeste(){
	carregarAlunos();
    carregarLivros();
    carregarEmprestimos();
	int opcao;
	string matricula; //nao tava funcionando declarando dentro do case (???)
	
	do{
		cout << "1 - Cadastrar aluno;" << endl;
		cout << "2 - Apagar aluno;" << endl;
		cout << "3 - Editar aluno;" << endl;
		cout << "4 - Listar todos os alunos;" << endl;
		cout << "5 - Cadastrar livro;" << endl;
		cout << "6 - Apagar livro;" << endl;
		cout << "7 - Editar livro;" << endl;
		cout << "8 - Listar todos os livros;" << endl;
		cout << "9 - Emprestar livro;" << endl;
		cout << "10 - Registrar Devolucao;" << endl;
		cout << "11 - Listar Emprestimos;" << endl;
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
                cadastrarLivro();
                break;
            
            case 6:
                limparTela();
                apagarLivro();
                break;
            	
            case 7:
            	limparTela();
            	editarLivro();
				break;

            case 8:
                limparTela();
                listarLivros();
                break;

            case 9:
                limparTela();
                registrarEmprestimo();
                break;

            case 10:
                limparTela();
                registrarDevolucao();
                break;

            case 11:
                limparTela();
                listarEmprestimos();
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
		
		//cin.ignore();
		limparTela();
		
	} while(opcao != 0);
	
}

int main(){
	menuTeste();
	
	return 0;
}
