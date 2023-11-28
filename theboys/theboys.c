// programa principal do projeto "The Boys - 2023"
// Autor: Victor Gabriel Rodrigues da Silva, GRR:20231959

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "set.h"
#include "lista.h"
#include "lef.h"

// seus #defines vão aqui
#define FIMDOMUNDO 525600
#define NHABILIDADES 10
#define NHEROIS 5 * NHABILIDADES
#define NBASES NHEROIS/6
#define NMISSOES FIMDOMUNDO/100

// minimize o uso de variáveis globais

struct heroi_h {
  int id ;                  // id do heroi
  int exp ;                 // experiencia total que o heroi obteve
  int paciencia ;           // paciencia do heroi
  int velocidade ;          // velocidade do heroi
  int Nhabilidade ;         // numero total de habilidades do heroi
  int *habilidades;         // vetor que vai ter as habilidades exatas do heroi
} ;

struct base_h {
  int id ;                  // id da base
  int local [2] ;           // coordenadas da base
  int lotacao ;             // herois totais que a base comporta
  struct set_t *presentes ; // herois presentes na base
  lista_t *fila ;           // fila de espera
} ;

struct missao_h {
  int id ;                  // id da missao 
  int local [2] ;           // coordenadas da missao
  int Nhabilidade ;         // numero de habilidades diferentes necessarias para a missao ser concluida
  int *habilidades ;        // vetor que tera as habilidades especificas 
} ;

struct mundo_h {
  int Nherois ;                            // numero total de herois que tem o mundo
  struct heroi_h *herois [NHEROIS] ;       // ponteiros para todos os herois
  int Nbases ;                             // numero total de bases que tem o mundo
  struct base_h *bases [NBASES] ;          // ponteiros para todas as bases
  int Nmissoes ;                           // numero total de missoes a acontecerem
  struct missao_h *missoes [NMISSOES] ;    // ponteiros para todas as missoes
  int Nhabilidades ;                       // numero de habilidades diferentes
  int TamanhoMundo [2] ;                   // dimensoes do mundo
  int relogio ;                            // relogio do mundo
} ;


// Funcao que aloca um heroi com suas caracteristicas
struct heroi_h *iniciaHeroi (int i)
{
  struct heroi_h *alocacao ;
  int Nhabilidades ;

  alocacao = malloc (sizeof (struct heroi_h)) ;

  alocacao->id = i ;
  alocacao->exp = 0 ;
  alocacao->paciencia = rand () % 101 ;
  alocacao->velocidade = 50 + rand () % 4951 ;

  Nhabilidades = 1 + rand () % 3 ;

  alocacao->Nhabilidade = Nhabilidades ;

  alocacao->habilidades = malloc (Nhabilidades * sizeof (int)) ;

  for (int i = 0; i < Nhabilidades; i++)
  {
    alocacao->habilidades[i] = 1 + rand () % 10 ;
    for (int j = 0; j < i; j++)
    {
      if (alocacao->habilidades[j] == alocacao->habilidades[i])
      {
        i-- ;
        break ;
      }
    }
  }

  return alocacao ;
}

// Funcao que desaloca o heroi alocado anteriormente
struct heroi_h *destroiHeroi (struct heroi_h *hero)
{
  if (hero == NULL)
    return NULL;

  free (hero->habilidades) ;
  free (hero) ;

  return NULL ;
}

// Funcao que aloca uma base com suas caracteristicas
struct base_h *iniciaBase (int i)
{
  struct base_h *alocacao ;

  alocacao =  malloc (sizeof (struct base_h)) ;

  alocacao->id = i ;
  alocacao->local[0] = rand () % 20000 ;
  alocacao->local[1] = rand () % 20000 ;
  alocacao->lotacao = 3 + rand () % 7 ;

  alocacao->presentes = set_create (alocacao->lotacao) ;

  alocacao->fila = lista_cria () ;

  return alocacao ;
}

// Funcao que desaloca uma base alocada anteriormente
struct base_h *destroiBase (struct base_h *base)
{
  if (base == NULL)
    return NULL;

  int elemento ;

  base->presentes = set_destroy (base->presentes) ;

  while (lista_vazia(base->fila) == 0)
  {
    lista_retira (base->fila, &elemento, 0) ;
  }
  
  base->fila = lista_destroi (base->fila) ;
  
  free (base) ;

  return NULL ;
}

// Funcao que aloca uma missao com suas caracteristicas
struct missao_h *iniciaMissao (int i)
{
  struct missao_h *alocacao ;
  int Nhabilidades ;

  alocacao =  malloc (sizeof (struct missao_h)) ;

  alocacao->id = i ;
  alocacao->local[0] = rand () % 20000 ;
  alocacao->local[1] = rand () % 20000 ;

  Nhabilidades = 6 + rand () % 4 ;

  alocacao->Nhabilidade = Nhabilidades ;

  alocacao->habilidades = malloc (Nhabilidades * sizeof (int)) ;

  for (int i = 0; i < Nhabilidades; i++)
  {
    alocacao->habilidades[i] = 1 + rand () % 10 ;
    for (int j = 0; j < i; j++)
    {
      if (alocacao->habilidades[j] == alocacao->habilidades[i])
      {
        i-- ;
        break ;
      }
    }
  }
  return alocacao ;
}

// Funcao que desaloca uma missao alocada anteriormente
struct missao_h *destroiMissao (struct missao_h *missao)
{
  if (missao == NULL)
    return NULL;

  free (missao->habilidades) ;
  free (missao) ;

  return NULL ;
}

// Funcao que aloca o mundo e chama as funcoes de alocar herois, bases e missoes
struct mundo_h *iniciaMundo ()
{
  struct mundo_h *alocacao ;
  int i ;

  alocacao = malloc (sizeof (struct mundo_h)) ;

  alocacao->Nherois = NHEROIS ;
  alocacao->Nbases = NBASES ;
  alocacao->Nmissoes = 5256 ;
  alocacao->Nhabilidades = 10 ;
  alocacao->relogio = 0 ;

  for (i = 0; i < NHEROIS; i++)
    alocacao->herois[i] = iniciaHeroi(i) ;

  for (i = 0; i < NBASES; i++)  
    alocacao->bases[i] = iniciaBase(i) ;

  for (i = 0; i < NMISSOES; i++)
    alocacao->missoes[i] = iniciaMissao(i) ;
  

  alocacao->TamanhoMundo[0] = 20000 ;
  alocacao->TamanhoMundo[1] = 20000 ;

  return alocacao ;

}

// Funcao usada para testar os dados das structs
void ImpressaoTeste (struct mundo_h *mundo)
{
  int i, j ;
  
  for (i = 0; i < NHEROIS; i++)
  {
    printf("%d %d %d %d %d ", mundo->herois[i]->id, mundo->herois[i]->exp, mundo->herois[i]->paciencia, mundo->herois[i]->velocidade, mundo->herois[i]->Nhabilidade) ;
    for (j = 0; j < mundo->herois[i]->Nhabilidade; j++)
      printf("%d ", mundo->herois[i]->habilidades[j]) ;
    printf ("\n") ;
  }

  printf("\n") ;

  for (i = 0; i < NBASES; i++)
  {
    printf("%d %d %d %d\n", mundo->bases[i]->id, mundo->bases[i]->local[0], mundo->bases[i]->local[1], mundo->bases[i]->lotacao) ;
  }

  printf("\n") ;

  for (i = 0; i < NMISSOES; i++)
  {
    printf("%d %d %d %d ", mundo->missoes[i]->id, mundo->missoes[i]->local[0], mundo->missoes[i]->local[1], mundo->missoes[i]->Nhabilidade) ;
    for (j = 0; j < mundo->missoes[i]->Nhabilidade; j++)
      printf("%d ", mundo->missoes[i]->habilidades[j]) ;
    printf ("\n") ;
  }
  
  return ;
}

// Evento chegada de um heroi a uma base
void eventoChega (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  
  int espera ;

  if (base->lotacao > base->presentes->num)
    espera = 1 ;
  else if (heroi->paciencia > (10 * base->fila->size))
    espera = 1 ;
  else espera = 0 ;

  if (espera == 1) 
  {
    ev = cria_evento (mundo->relogio, 1, heroi->id, base->id) ;
    insere_lef (l, ev) ;
    printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", mundo->relogio, heroi->id, base->id, base->presentes->num, base->lotacao) ;
  }
  else 
  {
    ev = cria_evento (mundo->relogio, 2, heroi->id, base->id) ;
    insere_lef (l, ev) ;
    printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", mundo->relogio, heroi->id, base->id, base->presentes->num, base->lotacao) ;
  }
}

// Evento espera de um heroi
void eventoEspera (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  

  printf ("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", mundo->relogio, heroi->id, base->id, base->fila->size) ;

  lista_insere (base->fila, heroi->id, -1) ;

  ev = cria_evento (mundo->relogio, 3, base->id, 0) ;
  insere_lef (l, ev) ;
}

// Evento de desistencia de um heroi
void eventoDesistencia (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  

  printf ("%6d: DESIST HEROI %2d BASE %d\n", mundo->relogio, heroi->id, base->id) ;

  ev = cria_evento (mundo->relogio, 6, heroi->id, base->id) ;
  insere_lef (l, ev) ;  
}

// Evento em que os herois sao avisado pelo porteiro
void eventoAvisa (struct mundo_h *mundo, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  
  int id ;

  printf ("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", mundo->relogio, base->id, base->presentes->num, base->lotacao) ;
  set_print (base->presentes) ;
  
  while (base->presentes->num < base->lotacao && base->fila->size > 0)
  {
    lista_retira (base->fila, &id, 0) ;
    set_add (base->presentes, id) ;
    printf ("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", mundo->relogio, base->id, id) ;
    ev = cria_evento (mundo->relogio, 4, id, base->id) ;
    insere_lef (l, ev) ; 
  }
}

// Evento do porteiro de avisar o heroi q ele entrara
void eventoEntra (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  
  int tpb ;

  tpb = 15 + heroi->paciencia * (1 + rand () % 20) ;

  printf ("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", mundo->relogio, heroi->id, base->id, base->presentes->num, base->lotacao, mundo->relogio + tpb) ;
 
  ev = cria_evento (mundo->relogio + tpb, 5, heroi->id, base->id) ;
  insere_lef (l, ev) ;  
}

// Evento em que o Heroi sai de uma base e ira para outra
void eventoSai (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  

  set_del (base->presentes, heroi->id) ;

  printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", mundo->relogio, heroi->id, base->id, base->presentes->num, base->lotacao) ;

  ev = cria_evento (mundo->relogio, 6, heroi->id, base->id) ;
  insere_lef (l, ev) ;  

  ev = cria_evento (mundo->relogio, 3, base->id, 0) ;
  insere_lef (l, ev) ;
}

// Evento que mostra quanto tempo o heroi demorara para ir de uma base a outra
void eventoViaja (struct mundo_h *mundo, struct heroi_h *heroi, struct base_h *base, struct lef_t *l )
{
  struct evento_t *ev ;  
  int baseNova, distancia ;
  
  baseNova = rand () % NBASES ;

  distancia = abs (base->local[0] - mundo->bases[baseNova]->local[0]) + abs (base->local[1] - mundo->bases[baseNova]->local[1]);

  printf ("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", mundo->relogio, heroi->id, base->id, baseNova, distancia, heroi->velocidade, mundo->relogio + (distancia/heroi->velocidade)) ;
  
  ev = cria_evento (mundo->relogio + (distancia/heroi->velocidade), 0, heroi->id, baseNova) ;
  insere_lef (l, ev) ;
  
}

// Evento missao 
void eventoMissao (struct mundo_h *mundo, struct lef_t *l, int *missoesCumpridas, int *tentativas)
{
  return ;

}

// Evento Fim que mostra todas estatisticas de herois e missoes
void eventoFim (struct mundo_h *mundo, int missoesCumpridas, int tentativas)
{
  float porcentagem, media ;

  printf ("%6d: FIM\n", mundo->relogio) ;

  for (int i = 0; i < mundo->Nherois; i++)
  {
    printf ("HEROI %2d PAC %3d VEL %4d EXP %4d HABS [ ", mundo->herois[i]->id, mundo->herois[i]->paciencia, mundo->herois[i]->velocidade, mundo->herois[i]->exp) ;
    for (int j = 0; j < mundo->herois[i]->Nhabilidade; j++)
      printf ("%d ", mundo->herois[i]->habilidades[i]) ;
    printf ("]\n") ;
  }

  porcentagem = missoesCumpridas ;
  media = tentativas ;

  printf ("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", missoesCumpridas, NMISSOES, (porcentagem/NMISSOES) * 100, media/NMISSOES) ;

}

// Funcao que organizara todos os eventos que acontecem durante a simulacao
void simulaMundo (struct mundo_h *mundo) 
{
  struct lef_t *l ;  
  struct evento_t *ev ;
  int i, missoesCumpridas, tentativas ;

  missoesCumpridas = 0 ;
  tentativas = 0 ;
  
  l = cria_lef () ;
  
  // Loops para inserir eventos na LEF
  // Missoes
  for (i = 0; i < NMISSOES; i++)
  {
    ev = cria_evento (rand () % FIMDOMUNDO, 7, 0, 1) ;
    insere_lef (l, ev) ;
  } 

  // Chegadas iniciais
  for (i = 0; i < 50; i++)
  {
    ev = cria_evento (rand () % 4320, 0, i, rand () % 8) ;
    insere_lef (l, ev) ;
  } 

  // Evento do fim
  ev = cria_evento (FIMDOMUNDO, 8, 0, 0) ;
  insere_lef (l, ev) ;

  // Looping para realizar os eventos
  while (mundo->relogio < FIMDOMUNDO)
  {
    ev = retira_lef (l) ;
    mundo->relogio = ev->tempo ;

    if (ev->tipo == 0)
    {
      eventoChega (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l) ;
    }
      
    else if (ev->tipo == 1)
    {  
      eventoEspera (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l)  ;
    }
      
    else if (ev->tipo == 2)
    {
      eventoDesistencia (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l)  ;
    }
      
    else if (ev->tipo == 3)
    {
      eventoAvisa (mundo, mundo->bases[ev->dado1], l)  ;
    }
      
    else if (ev->tipo == 4)
    {
      eventoEntra (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l)  ;
    }
      
    else if (ev->tipo == 5)
    {
      eventoSai (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l)  ;
    }
      
    else if (ev->tipo == 6)
    {
      eventoViaja (mundo, mundo->herois[ev->dado1], mundo->bases[ev->dado2], l)  ;
    }
      
    else if (ev->tipo == 7)
    {
      eventoMissao (mundo, l, &missoesCumpridas, &tentativas) ;
    }
    
    else if (ev->tipo == 8)
    {
      eventoFim(mundo, missoesCumpridas, tentativas) ;
    }

    destroi_evento (ev) ;
  }

  // Looping pra retirar possiveis eventos que so aconteceriam apos o final da simulacao 
  while (!vazia_lef (l))
  {
    ev = retira_lef(l) ;
    destroi_evento (ev) ;
  }

  // Desalocar LEF  
  l = destroi_lef (l) ;
  
}

// Funcao que vai desalocar todos herois, bases, missoes e o mundo durante a execucao da simulacao
struct mundo_h *destroiMundo (struct mundo_h *mundo)
{
  int i ;
  
  if (mundo == NULL)
    return NULL;

  for (i = 0; i < NHEROIS; i++)
    destroiHeroi(mundo->herois[i]) ;
  

  for (i = 0; i < NBASES; i++)
    destroiBase(mundo->bases[i]) ;
  

  for (i = 0; i < NMISSOES; i++)
    destroiMissao(mundo->missoes[i]) ;

  free (mundo) ;
  return NULL ;
}

void testaMundo (struct mundo_h mundo)
{

  struct lef_t *l ;  

    l = cria_lef () ;

      eventoChega (mundo, mundo->herois[1], mundo->bases[1], l) ;
    
    
      eventoEspera (mundo, mundo->herois[1], mundo->bases[1], l)  ;

      eventoDesistencia (mundo, mundo->herois[1], mundo->bases[1], l)  ;

      eventoAvisa (mundo, mundo->bases[1], l)  ;

      eventoEntra (mundo, mundo->herois[1], mundo->bases[1], l)  ;

      eventoSai (mundo, mundo->herois[1], mundo->bases[1], l)  ;
      
      eventoViaja (mundo, mundo->herois[1], mundo->bases[1], l)  ;

      eventoFim(mundo, 6, 7) ;

}
  

// programa principal
int main ()
{
  struct mundo_h *world ;
  
  srand(0) ;
  
  // iniciar o mundo
  world = iniciaMundo() ;
  
  // executar o laço de simulação
  // simulaMundo (world) ;

  testaMundo (world) ;
  
  // destruir o mundo
  world = destroiMundo (world) ;
  
  return (0) ;
}


