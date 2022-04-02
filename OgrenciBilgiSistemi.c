#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct dersler{
	char ders_kodu[8];
	char ders_adi[100];
	unsigned int kredi :5;
	unsigned int kontenjan;
	unsigned short kayitli_ogr;
	int *ogrenciler;
	struct dersler *next;
}DERSLER;
typedef struct ogrenciler{
	struct ogrenciler *prev;
	int no;
	char ad[30];
	char soyad[30];
	unsigned short ders_sayisi;
	unsigned short kredi_sayisi;
	struct ogrenciler *next;
}OGRENCILER;
typedef struct ogrenciDersKayit{
	int id;
	int ogr_no;
	char ders_kod[8];
	char tarih[20];
	char durum[20];
	struct ogrenciDersKayit *next;
}OGRENCIDERS;
OGRENCIDERS *ogrenciDersOlustur(){
	OGRENCIDERS *ogrders;
	ogrders = (OGRENCIDERS*)malloc(sizeof(OGRENCIDERS));
	if(ogrders == NULL){
		printf("Hata:Ogrenci-Ders kaydi olusturulamadi.\n");
	}
	ogrders->next = NULL;
	return ogrders;
}
void ogrenciDersKayit(OGRENCIDERS **head,char *kod,int no,const char *tarih,OGRENCIDERS *(*ogrenciDersOlustur)()){
	OGRENCIDERS *ogrders,*p;
	ogrders = ogrenciDersOlustur();
	if((*head) == NULL){
		(*head) = ogrders;
		ogrders->id = 1;
	}
	else{
		p = (*head);
		while(p->next!=NULL){
			p = p->next;
		}
		p->next = ogrders;
		ogrders->id = p->id + 1;
	}
	strcpy(ogrders->ders_kod,kod);
	ogrders->ogr_no = no;
	strcpy(ogrders->tarih,tarih);
	strcpy(ogrders->durum,"KAYITLI");
}
void ogrenciDersGuncelle(OGRENCIDERS *head,char *kod , int no , const char *durum){
	OGRENCIDERS *p;
	p = head;
	while(p!=NULL && (strcmp(p->ders_kod,kod)!=0 || p->ogr_no != no)){
		p = p->next;
	}
	strcpy(p->durum,durum);
}
void ogrenciDersDosya(OGRENCIDERS *head){
	OGRENCIDERS *p;
	FILE *fp;
	p = head;
	fp = fopen("ÖðrenciDersKayýt.txt","w");
	//printf("Dosya kaydina girdi\n");
	for(p;p!=NULL;p=p->next){
		fprintf(fp,"%d,%d,%s,%s,%s\n",p->id,p->ogr_no,p->ders_kod,p->tarih,p->durum);
	}
	fclose(fp);
}
void ogrenciDersYazdir(OGRENCIDERS *head){
	OGRENCIDERS *p;
	p = head;
	printf("Ogrenci ders kayit listesi:\n");
	for(p;p!=NULL;p = p->next){
		printf("%d <%d-%s> %s %s\n",p->id,p->ogr_no,p->ders_kod,p->tarih,p->durum);
	}
}
OGRENCILER *ogrenciOlustur(){
	OGRENCILER *ogr;
	ogr = (OGRENCILER *)malloc(sizeof(OGRENCILER));
	if(ogr == NULL){
		printf("Hata:Ogrenci olusturulamadi\n");
	}
	printf("Eklenecek ogrencinin numarasini giriniz :");
	scanf("%d",&ogr->no);
	printf("Ogrenci adini giriniz :");
	scanf("%s",ogr->ad);
	printf("Ogrenci soyadini giriniz :");
	scanf("%s",ogr->soyad);
	printf("Ogrencinin kayitli oldugu toplam ders sayisini giriniz :");
	scanf("%hu",&ogr->ders_sayisi);
	printf("Ogrencinin kayitli oldugu toplam kredi sayisini giriniz :");
	scanf("%hu",&ogr->kredi_sayisi);
	ogr->next = NULL;
	ogr->prev = NULL;
	return ogr;
}
void ogrenciEkle(OGRENCILER**head){
	OGRENCILER *ogr,*p,*tmp;
	ogr = ogrenciOlustur();
	if((*head) != NULL){
		p = (*head);
		if(ogr->no<p->no){
			(*head) = ogr;
			ogr->next = p;
			p->prev = ogr;
			//printf("Section1(ogrenciekle)\n");
		}
		else if(ogr->no!=p->no){
			while(p->next!=NULL && p->next->no < ogr->no && p->next->no!=ogr->no){
				p = p->next;
			}
			if(p->next == NULL){
				p->next = ogr;
				ogr->prev = p;
			}
			else if(p->next->no > ogr->no){
				//printf("Section2(ogrenciekle)\n");
				tmp = p->next;
				ogr->next = tmp;
				tmp->prev = ogr;
				p->next = ogr;
				ogr->prev = p;
			}
			else{
				printf("Eklemek istediginiz ogrenci zaten mevcuttur.\n");
			}
		}
		else{
			printf("Eklemek istediginiz ogrenci zaten mevcuttur.\n");
		}
	}
	else{
		//printf("Ýlk deger null\n");
		(*head) = ogr;
	}
}
OGRENCILER *ogrenciBul(OGRENCILER *head,int no){
	OGRENCILER *p = head;
	while(p!=NULL && p->no!=no){
		p = p->next;
	}
	return p;
}
void ogrenciSil(OGRENCILER **head,DERSLER *head_ders){
	OGRENCILER *ogr,*p=(*head);
	DERSLER *ders=head_ders;
	int no,i=0;
	printf("Silinecek ogrencinin numarasini giriniz :");
	scanf("%d",&no);
	ogr = ogrenciBul((*head),no);
	if(ogr==NULL){
		printf("Silmek istediginiz ogrenci kayitlarda bulunmamaktadir\n");
	}
	else{
		while(ders!=NULL){
			while(ders->ogrenciler[i]!=no && i<ders->kayitli_ogr){
				i++;
			}
			if(i!=ders->kayitli_ogr){
				for(i;i<ders->kayitli_ogr-1;i++){
					ders->ogrenciler[i] = ders->ogrenciler[i+1];
				}
				ders->ogrenciler = (int *)realloc(ders->ogrenciler,(ders->kayitli_ogr-1)*sizeof(int));
				ders->kayitli_ogr = ders->kayitli_ogr - 1;
			}
			ders = ders->next;
			i=0;
		}
		if(p==ogr){
			(*head) = p->next;
			free(p);
		}
		else{
			while(p->next!=ogr){
				p = p->next;
			}
			if(p->next->next==NULL){
				p->next = ogr->next;
				free(ogr);
			}
			else{
				p->next = ogr->next;
				ogr->next->prev = p;
				free(ogr);
			}
		}
	}
}
void ogrenciListesi(OGRENCILER *head){
	OGRENCILER *ogr=head;
	printf("Mevcut tum ogrenciler :\n");
	for(ogr;ogr!=NULL;ogr=ogr->next){
		printf("Ogrenci numarasi : %d\n",ogr->no);
		printf("Ogrenci adi : %s\n",ogr->ad);
		printf("Ogrenci soyadi : %s\n",ogr->soyad);
		printf("Ogrencinin kayitli kredi sayisi : %hu\n",ogr->kredi_sayisi);
		printf("Ogrencinin kayitli ders sayisi : %hu\n",ogr->ders_sayisi);
	}
}
void ogrenciKaydet(OGRENCILER *ogr){
	FILE *fp;
	fp = fopen("Öðrenciler.txt","w");
	for(ogr;ogr!=NULL;ogr=ogr->next){
		fprintf(fp,"%d %s %s %hu %hu\n",ogr->no,ogr->ad,ogr->soyad,ogr->kredi_sayisi,ogr->ders_sayisi);
	}
	fclose(fp);
}
void dersYazdir(DERSLER *p){
	printf("%s kodlu dersin adi : %s\nkredisi: %u\nkontenjani: %u\n",p->ders_kodu,p->ders_adi,p->kredi,p->kontenjan);
}
void dersListesi(DERSLER *head){
	DERSLER *ders=head;
	printf("Mevcut tum dersler :\n");
	for(ders;ders!=NULL;ders=ders->next){
		printf("Ders kodu :%s\n",ders->ders_kodu);
		printf("Ders adi :%s\n",ders->ders_adi);
		printf("Ders kredisi : %u\n",ders->kredi);
		printf("Ders kontenjani :%u\n",ders->kontenjan);
	}
}
DERSLER *dersOlustur(){
	DERSLER *ders;
	int kred;
	ders =(DERSLER*)malloc(sizeof(DERSLER));
	if(ders == NULL){
		printf("Hata:Ders olusturulamadi\n");
	}
	printf("Eklenecek ders kodunu giriniz :");
	scanf("%s",ders->ders_kodu);
	printf("Ders adini giriniz :");
	scanf("%s",ders->ders_adi);
	printf("Ders kredi miktarini giriniz :");
	scanf("%d",&kred);
	ders->kredi = kred;
	printf("Ders kontenjani giriniz :");
	scanf("%d",&ders->kontenjan);
	ders->next = NULL;
	ders->kayitli_ogr = 0;
	ders->ogrenciler = NULL;
	//printf("Ders olusturuldu\n");
	return ders;
}
void dersEkle(DERSLER **head){
	DERSLER *ders,*d,*tmp;
	ders = dersOlustur();
	if((*head)!=NULL){
		d = (*head);
		if(strcmp(d->ders_kodu,ders->ders_kodu)>0){
			(*head) = ders;
			ders->next = d;
		}
		else if(strcmp(d->ders_kodu,ders->ders_kodu)!=0){
			while(d->next!=NULL && strcmp(d->next->ders_kodu,ders->ders_kodu)<0 && strcmp(d->next->ders_kodu,ders->ders_kodu)!=0){
				d=d->next;
			}
			if(d->next == NULL){
				d->next = ders;
			}
			else if(strcmp(d->next->ders_kodu,ders->ders_kodu)!=0){
				tmp = d->next;
				ders->next = tmp;
				d->next = ders;	
			}
			else{
				printf("Eklemek istediginiz ders zaten mevcuttur.\n");
			}
		}
		else{
			printf("Eklemek istediginiz ders zaten mevcuttur.\n");
		}
	}
	else{
		(*head) = ders;
	}
}
DERSLER *dersBul(DERSLER *head,char*kod){
	DERSLER *d = head;
	while(d!=NULL && strcmp(d->ders_kodu,kod)!=0){
		d = d->next;
	}
	return d;
}
void ogrenciDersSil(OGRENCIDERS *head_ogrders,DERSLER *head_ders,char *kod){
	OGRENCIDERS *ogrders = head_ogrders;
	DERSLER *ders;
	ders = dersBul(head_ders,kod);
	while(ogrders!=NULL){
		if(strcmp(ogrders->ders_kod,kod)==0){
			strcpy(ogrders->durum,"DERS_KAPANDI");
		}
		ogrders = ogrders->next;
	}
}
void dersSil(DERSLER **head,OGRENCIDERS *head_ogrders,OGRENCILER *head_ogr){
	DERSLER *d,*ders;
	OGRENCILER *ogr;
	char kod[8];
	int i;
	printf("Silinecek dersin kodunu giriniz :");
	scanf("%s",kod);
	ders = dersBul((*head),kod);
	if(ders==NULL){
		printf("Silmek istediginiz ders kayitlarda bulunmamaktadir.\n");
	}
	else{
		ogrenciDersSil(head_ogrders,(*head),kod);
		for(i=0;i<ders->kayitli_ogr;i++){
			ogr = ogrenciBul(head_ogr,ders->ogrenciler[i]);
			ogr->ders_sayisi = ogr->ders_sayisi - 1;
			ogr->kredi_sayisi = ogr->kredi_sayisi - ders->kredi;
		}
		d = (*head);
		if(d==ders){
			(*head) = ders->next;
			free(ders);
		}
		else{
			while(d->next!=ders){
				d = d->next;
			}
			d->next = ders->next;
			free(ders);
		}
	}
}
void dersKaydet(DERSLER *ders){
	FILE *fp;
	fp = fopen("Dersler.txt","w");
	for(ders;ders!=NULL;ders=ders->next){
		fprintf(fp,"%s %s %u %u\n",ders->ders_kodu,ders->ders_adi,ders->kredi,ders->kontenjan);
	}
	fclose(fp);
}
void dersKayit(OGRENCILER *head_ogr,DERSLER *head_ders,void(*f)(),OGRENCIDERS**head_ogrders,OGRENCIDERS*(*odo)(),void (*odk)(),int max_ders,int max_kredi){
	OGRENCILER *ogr;
	DERSLER *ders;
	int no,i=0,tmp,secim=0;
	char kod[8],tarih[20];
	printf("Ders kaydi yapilacak ogrencinin numarasini giriniz :");
	scanf("%d",&no);
	ogr = ogrenciBul(head_ogr,no);
	if(ogr==NULL){
		printf("Girilen ogrenci kaydi sistemde bulunmamaktadir.\n");
	}
	else{
		do{
			if(ogr->ders_sayisi<max_ders){
				printf("Kaydolunacak ders kodunu giriniz :");
				scanf("%s",kod);
				ders = dersBul(head_ders,kod);
				//printf("Debug 1\n");
				if(ders==NULL){
					printf("Girilen ders kaydi sistemde bulunmamaktadir.\n");
				}
				else if(ders->kayitli_ogr==ders->kontenjan){
					printf("Kaydolunmak istenen dersin kontenjani dolmustur.\n");
				}
				else if(ogr->kredi_sayisi+ders->kredi<=max_kredi){
					//printf("Debug 2\n");
					if(ders->kayitli_ogr == 0){
						//printf("Debug 3\n");
						ders->ogrenciler = (int *)malloc(sizeof(int));
						ders->ogrenciler[0] = no;
						ders->kayitli_ogr = 1;
						ogr->kredi_sayisi += ders->kredi;
						ogr->ders_sayisi += 1;
						printf("Kayit tarihini giriniz (gun/ay/yil) :");
						scanf("%s",tarih);
						odk(head_ogrders,kod,no,tarih,odo);
						f(ders);
					}
					else{
						ders->ogrenciler = (int*)realloc(ders->ogrenciler,(1+ders->kayitli_ogr)*sizeof(int));
						while(ders->ogrenciler[i]<no && i<ders->kayitli_ogr){
							i++;
						}
						if(ders->ogrenciler[i] == no){
							printf("Girilen ogrenci zaten bu derse kayitlidir.\n");
						}
						else if(i==ders->kayitli_ogr){
							ders->ogrenciler[i] = no;
							printf("Kayit tarihini giriniz (gun/ay/yil) :");
							scanf("%s",tarih);
							ders->kayitli_ogr = ders->kayitli_ogr + 1;
							ogr->kredi_sayisi += ders->kredi;
							ogr->ders_sayisi += 1;
							odk(head_ogrders,kod,no,tarih,odo);
							f(ders);
						}
						else{
							tmp = i;
							for(i;i<ders->kayitli_ogr;i++){
								ders->ogrenciler[i+1] = ders->ogrenciler[i];
							}
							ders->ogrenciler[tmp] = no;
							printf("Kayit tarihini giriniz (gun/ay/yil) :");
							scanf("%s",tarih);
							ders->kayitli_ogr = ders->kayitli_ogr + 1;
							ogr->kredi_sayisi += ders->kredi;
							ogr->ders_sayisi += 1;
							odk(head_ogrders,kod,no,tarih,odo);
							f(ders);
						}

					}
				}
				else{
					printf("Ogrencinin bu derse kaydolmak icin yeterli kredisi bulunmamaktadir.\n");
				}
			}
			else{
				printf("Ogrenci maksimum ders sayisina ulasmistir.\n");
			}
			printf("Eklemek istediginiz baska bir ders varsa 1\nCikmak icin 2 tuslayiniz :");
			scanf("%d",&secim);
		}while(secim==1);	
	}
}
void dersBirak(OGRENCILER *head_ogr,DERSLER *head_ders,void(*f)(),OGRENCIDERS *head_ogrders,void (*odg)()){
	OGRENCILER *ogr;
	DERSLER *ders;
	int no,i=0,secim=1;
	char kod[8];
	printf("Ders birakmak isteyen ogrencinin numarasini giriniz :");
	scanf("%d",&no);
	ogr = ogrenciBul(head_ogr,no);
	if(ogr == NULL){
		printf("Girilen ogrenci kaydi sistemde bulunmamaktadir.\n");
	}
	else{
		do{
			printf("Ogrencinin birakmak istedigi dersin kodunu giriniz :");
			scanf("%s",kod);
			ders = dersBul(head_ders,kod);
			if(ders == NULL){
				printf("Girilen ders kaydi sistemde bulunmamaktadir.\n");
			}
			else if(ders->ogrenciler!=NULL){
				while(ders->ogrenciler[i]!=no && i<ders->kayitli_ogr){
					i++;
				}
				if(i==ders->kayitli_ogr){
					printf("Ogrenci zaten girilen derse kayitli degildir.\n");
				}
				else{
					for(i;i<ders->kayitli_ogr-1;i++){
						ders->ogrenciler[i] = ders->ogrenciler[i+1];
					}
					ders->ogrenciler = (int*)realloc(ders->ogrenciler,(ders->kayitli_ogr-1)*sizeof(int));
					ders->kayitli_ogr = ders->kayitli_ogr - 1;
					ogr->kredi_sayisi = ogr->kredi_sayisi - ders->kredi ;
					ogr->ders_sayisi = ogr->ders_sayisi - 1;
					odg(head_ogrders,ders->ders_kodu,no,"BIRAKTI");
					f(ders);
				}
			}
			else{
				printf("Bu derse kayitli ogrenci bulunmamaktadir.\n");
			}
			printf("Tekrar ders birakmak icin 1\nCikmak icin 2 tuslayiniz :");
			scanf("%d",&secim);	
		}while(secim==1);
	}
}
void dersogrYazdir(DERSLER *ders){
	int i;
	printf("%s kodlu derse kayitli ogrenciler :\n");
	for(i=0;i<ders->kayitli_ogr;i++){
		printf("%d\n",ders->ogrenciler[i]);
	}
}
void dersTemizle(DERSLER *head){
	DERSLER *tmp;
	while(head!=NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
void ogrTemizle(OGRENCILER *head){
	OGRENCILER *tmp;
	while(head!=NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
void dersOgrenciTemizle(OGRENCIDERS *head){
	OGRENCIDERS *tmp;
	while(head!=NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
void split_ders(DERSLER* ders,char buff[255],char delim[2]){
	char *token;
	token=strtok(buff,delim);
	strcpy(ders->ders_kodu,token);
	token=strtok(NULL,delim);
	strcpy(ders->ders_adi,token);
	token=strtok(NULL,delim);
	ders->kredi=atoi(token);
	token=strtok(NULL,delim);
	ders->kontenjan=atoi(token);
	ders->next = NULL;
	ders->kayitli_ogr=0;
	ders->ogrenciler = NULL;
}
DERSLER *dersOku(void(*split_ders)()){
	DERSLER *head_ders,*prev,*curr;
	char buff[255],delim[2]=" ";
	FILE *fp;
	fp = fopen("Dersler.txt","r");
	if(fgets(buff,255,fp)!=NULL){
		prev = (DERSLER *)malloc(sizeof(DERSLER));
		split_ders(prev,buff,delim);
		head_ders = prev;
	}
	else{
		return NULL;
	}
	while(fgets(buff,255,fp)!=NULL){
		curr = (DERSLER *)malloc(sizeof(DERSLER));
		split_ders(curr,buff,delim);
		prev->next = curr;
		prev = curr;
	}
	return head_ders;
}
void split_ogr(OGRENCILER *ogr,char buff[255],char delim[2]){
	char *token;
	token=strtok(buff,delim);
	ogr->no = atoi(token);
	token=strtok(NULL,delim);
	strcpy(ogr->ad,token);
	token=strtok(NULL,delim);
	strcpy(ogr->soyad,token);
	token=strtok(NULL,delim);
	ogr->kredi_sayisi = atoi(token);
	token=strtok(NULL,delim);
	ogr->ders_sayisi = atoi(token);
	ogr->next = NULL;
}
OGRENCILER *ogrOku(void(*split_ogr)()){
	OGRENCILER *head_ogr,*prev,*curr;
	char buff[255],delim[2]=" ";
	FILE *fp;
	fp = fopen("Öðrenciler.txt","r");
	if(fgets(buff,255,fp)!=NULL){
		prev = (OGRENCILER *)malloc(sizeof(OGRENCILER));
		split_ogr(prev,buff,delim);
		head_ogr = prev;
	}
	else{
		return NULL;
	}
	while(fgets(buff,255,fp)!=NULL){
		curr = (OGRENCILER *)malloc(sizeof(OGRENCILER));
		split_ogr(curr,buff,delim);
		prev->next = curr;
		prev = curr;
	}
	return head_ogr;
}
void split_ogrders(OGRENCIDERS *ogrders,char buff[255],char delim[2]){
	char *token;
	token = strtok(buff,delim);
	ogrders->id = atoi(token);
	//printf("%d ",ogrders->id);
	token = strtok(NULL,delim);
	ogrders->ogr_no = atoi(token);
	//printf("%d ",ogrders->ogr_no);
	token = strtok(NULL,delim);
	strcpy(ogrders->ders_kod,token);
	//printf("%s ",ogrders->ders_kod);
	token = strtok(NULL,delim);
	strcpy(ogrders->tarih,token);
	//printf("%s ",ogrders->tarih);
	token = strtok(NULL,delim);
	token[strlen(token)-1]='\0';
	strcpy(ogrders->durum,token);
	//printf("%s",ogrders->durum);
	ogrders->next = NULL;
}
OGRENCIDERS *ogrdersOku(void(*split_ogrders)()){
	OGRENCIDERS *head_ogrders,*prev,*curr;
	FILE *fp;
	char buff[255],delim[2]=",";
	fp = fopen("ÖðrenciDersKayýt.txt","r");
	if(fgets(buff,255,fp)!=NULL){
		prev = (OGRENCIDERS *)malloc(sizeof(OGRENCIDERS));
		split_ogrders(prev,buff,delim);
		head_ogrders = prev;
	}
	else{
		return NULL;
	}
	while(fgets(buff,255,fp)!=NULL){
		curr = (OGRENCIDERS *)malloc(sizeof(OGRENCIDERS));
		split_ogrders(curr,buff,delim);
		prev->next = curr;
		prev = curr;
	}
	return head_ogrders;
}
void dersGuncelle(OGRENCIDERS *head_ogrders,DERSLER *head_ders){
	OGRENCIDERS *ogrders;
	ogrders = head_ogrders;
	DERSLER *ders;
	int i=0,tmp;
	while(ogrders!=NULL){
		if(strcmp(ogrders->durum,"KAYITLI")==0){
			ders = dersBul(head_ders,ogrders->ders_kod);
			if(ders->kayitli_ogr==0){
				ders->ogrenciler = (int *)malloc(sizeof(int));
				ders->ogrenciler[0] = ogrders->ogr_no;
				ders->kayitli_ogr = ders->kayitli_ogr + 1;
			}
			else{
				ders->ogrenciler = (int *)realloc(ders->ogrenciler,(1+ders->kayitli_ogr)*sizeof(int));
				while(ders->ogrenciler[i]<ogrders->ogr_no && i<ders->kayitli_ogr){
							i++;
				}
				tmp = i;
				for(i;i<ders->kayitli_ogr;i++){
					ders->ogrenciler[i+1] = ders->ogrenciler[i];
				}
				ders->ogrenciler[tmp] = ogrders->ogr_no;
				ders->kayitli_ogr = ders->kayitli_ogr + 1;
			}
		}
		ogrders = ogrders->next;
	}
}
void dersProg(DERSLER *head_ders,OGRENCILER *head_ogr){
	DERSLER *p=head_ders;
	OGRENCILER *ogr;
	FILE *fp;
	char filename[8];
	int i;
	while(p!=NULL){
		sprintf(filename,"%s.txt",p->ders_kodu);
		fp = fopen(filename,"w");
		for(i=0;i<p->kayitli_ogr;i++){
			fp = fopen(filename,"a");
			ogr = ogrenciBul(head_ogr,p->ogrenciler[i]);
			fprintf(fp,"%d %s %s %hu %hu\n",ogr->no,ogr->ad,ogr->soyad,ogr->kredi_sayisi,ogr->ders_sayisi);
		}
		p=p->next;
	}
	fclose(fp);
}
void ogrenciProg(OGRENCIDERS *head_ogrders,OGRENCILER *head_ogr,DERSLER *head_ders){
	OGRENCIDERS *ogrders=head_ogrders;
	OGRENCILER *ogr;
	DERSLER *ders;
	FILE *fp;
	char filename[22];
	int no;
	printf("Ders programi yazdirilacak ogrencinin numarasini giriniz :");
	scanf("%d",&no);
	ogr = ogrenciBul(head_ogr,no);
	if(ogr!=NULL){
		printf("%d numarali ogrencinin ders programi:\n",no);
		sprintf(filename,"%d_DERSPROGRAMI.txt",no);
		fp = fopen(filename,"w");
		while(ogrders!=NULL){
			if(ogrders->ogr_no==no){
				if(strcmp(ogrders->durum,"KAYITLI")==0){
					fp = fopen(filename,"a");
					ders = dersBul(head_ders,ogrders->ders_kod);
					fprintf(fp,"%s %s %u %u\n",ders->ders_kodu,ders->ders_adi,ders->kredi,ders->kontenjan);
					printf("%s %s %u %u\n",ders->ders_kodu,ders->ders_adi,ders->kredi,ders->kontenjan);
				}
			}
			ogrders = ogrders->next;
		}
		fclose(fp);
	}
	else{
		printf("Girilen ogrenci kayitlarda bulunmamaktadir.\n");
	}
}
int main(){
	DERSLER *head_ders = dersOku(split_ders);
	OGRENCILER *head_ogrenci = 	ogrOku(split_ogr);
	OGRENCIDERS *head_ogrders = ogrdersOku(split_ogrders);
	dersGuncelle(head_ogrders,head_ders);
	int max_ders,max_kredi;
	dersListesi(head_ders);
	printf("\n");
	ogrenciListesi(head_ogrenci);
	printf("\n");
	ogrenciDersYazdir(head_ogrders);
	printf("\n");
	int secim;
	printf("Hosgeldiniz\n");
	printf("Butun ogrenciler icin maksimum secilebilecek ders sayisini giriniz :");
	scanf("%d",&max_ders);
	printf("Butun ogrenciler icin maksimum alinabilecek kredi sayisini giriniz :");
	scanf("%d",&max_kredi);
	do{
		printf("\nDers eklemek icin (1)\nDers silmek icin (2)\nOgrenci eklemek icin (3)\nOgrenci silmek icin (4)\n");
		printf("Ogrenci ders kaydi icin (5)\nOgrenci ders birakma islemi icin (6)\nHer derse ait ders programi dosyasini olusturmak icin (7)\nOgrenci ders programi dosyasi olusturmak icin (8)\nProgrami sonlandirmak icin (0) tuslayiniz :");
		scanf("%d",&secim);
		switch(secim){
			case 0:
				break;
			case 1:
				dersEkle(&head_ders);
				dersKaydet(head_ders);
				printf("\n");
				dersListesi(head_ders);
				break;
			case 2:
				dersSil(&head_ders,head_ogrders,head_ogrenci);
				dersKaydet(head_ders);
				ogrenciDersDosya(head_ogrders);
				ogrenciKaydet(head_ogrenci);
				printf("\n");
				dersListesi(head_ders);
				break;
			case 3:
				ogrenciEkle(&head_ogrenci);
				ogrenciKaydet(head_ogrenci);
				printf("\n");
				ogrenciListesi(head_ogrenci);
				break;
			case 4:
				ogrenciSil(&head_ogrenci,head_ders);
				ogrenciKaydet(head_ogrenci);
				printf("\n");
				ogrenciListesi(head_ogrenci);
				break;
			case 5:
				dersKayit(head_ogrenci,head_ders,dersogrYazdir,&head_ogrders,ogrenciDersOlustur,ogrenciDersKayit,max_ders,max_kredi);
				ogrenciDersDosya(head_ogrders);
				dersKaydet(head_ders);
				ogrenciKaydet(head_ogrenci);
				ogrenciDersYazdir(head_ogrders);
				break;
			case 6:
				dersBirak(head_ogrenci,head_ders,dersogrYazdir,head_ogrders,ogrenciDersGuncelle);
				ogrenciDersDosya(head_ogrders);
				dersKaydet(head_ders);
				ogrenciKaydet(head_ogrenci);
				ogrenciDersYazdir(head_ogrders);
				break;
			case 7:
				dersProg(head_ders,head_ogrenci);
				break;
			case 8:
				ogrenciProg(head_ogrders,head_ogrenci,head_ders);
				break;
			default:
				printf("\nLutfen gecerli bir secenek giriniz (0,1,2,3,4,5,6,7,8)\n");
		}
	}while(secim!=0);
	ogrTemizle(head_ogrenci);
	dersTemizle(head_ders);
	dersOgrenciTemizle(head_ogrders);
	return 0;
}
