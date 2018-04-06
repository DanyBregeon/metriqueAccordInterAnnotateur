import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import org.jopendocument.dom.spreadsheet.Sheet;
import org.jopendocument.dom.spreadsheet.SpreadSheet;

public class Main {
	//mettre le chemin du fichier ici
	final static String nomDuFichier = "ods/TESTACCORD_EMOTION.ods";
	//indiquer le numero de la feuille qui nous intéresse
	final static int numFeuille = 1;
	//indiquer le nombre de colonnes et de lignes de la feuille
	final static int nbColonne = 28;
	final static int nbLigne = 139;
	//indiquer à partir de quelle ligne et de quelle colonne les données commencent
	final static int premiereColonne = 1;
	final static int premiereLigne = 1;
	//indiquer le nombre de classe
	final static int nbc = 3;
	//indiquer le nombre d'annotateur par fichier créé
	final static int nban = 9;
	//indiquer le nom du fichier créé
	final static String nomFichierTableau = "EmotionContexte";

	public static void main(String[] args) {
		
		try{
			File f = new File(nomDuFichier);
			
			final Sheet sheet = SpreadSheet.createFromFile(f).getSheet(numFeuille);
			
			int nba = nbColonne-premiereColonne;
			int nbobs = nbLigne-premiereLigne;
			int numFile = 0;
			
			//String[][] tab = new String[nba][nbobs];
			String[][] tab = new String[nba][nbobs];
			
			for (int col = premiereColonne; col < nbColonne; col++) {
				for (int lig = premiereLigne; lig < nbLigne; lig++) {
					//System.out.print(sheet.getImmutableCellAt(col, lig).getTextValue() + "	");
					tab[col-premiereColonne][lig-premiereLigne] = sheet.getImmutableCellAt(col, lig).getTextValue();
					if(nbc == 3){
						if(tab[col-premiereColonne][lig-premiereLigne].equals("-2")){
							tab[col-premiereColonne][lig-premiereLigne] = "-1";
						}else if(tab[col-premiereColonne][lig-premiereLigne].equals("2")){
							tab[col-premiereColonne][lig-premiereLigne] = "1";
						}
					}
				}
				//System.out.println();
			}
			if(nba == 27){
				String[][] tab1 = new String[nban][nbobs];
				String[][] tab2 = new String[nban][nbobs];
				String[][] tab3 = new String[nban][nbobs];
				delimiterAnnotateur(0, 8, tab, tab1);
				delimiterAnnotateur(9, 17, tab, tab2);
				delimiterAnnotateur(18, 26, tab, tab3);
				writeTabCsv(nban, nbc, nbobs, tab1, 0);
				writeTabCsv(nban, nbc, nbobs, tab2, 1);
				writeTabCsv(nban, nbc, nbobs, tab3, 2);
			}else if(nba == 25){
				String[][] tab1 = new String[nban][nbobs];
				String[][] tab2 = new String[nban][nbobs];
				String[][] tab3 = new String[nban][nbobs];
				delimiterAnnotateur(0, 8, tab, tab1);
				delimiterAnnotateur(8, 16, tab, tab2);
				delimiterAnnotateur(16, 24, tab, tab3);
				writeTabCsv(nban, nbc, nbobs, tab1, 0);
				writeTabCsv(nban, nbc, nbobs, tab2, 1);
				writeTabCsv(nban, nbc, nbobs, tab3, 2);
			}else{
				writeTabCsv(nba, nbc, nbobs, tab, numFile);
			}
			
			
		}catch(IOException e){
			e.printStackTrace();
		}	
	}
	
	public static void delimiterAnnotateur(int min, int max, String[][] tab, String[][] tab2){
		for(int i=0; i<=max-min; i++){
			for (int j = 0; j < tab[0].length; j++){
				tab2[i][j] = tab[min+i][j];
			}
		}
	}
	
	public static void writeTabCsv(int nba, int nbc, int nbobs, String[][] tab, int nbFile) {
		String chemin = "tableaux/classe" + nbc + "/Tableau" + nomFichierTableau + nbFile + ".csv";
        File fichier =new File(chemin); 
        try {
            // Creation du fichier
            fichier .createNewFile();
            // creation d'un writer (un �crivain)
            final FileWriter writer = new FileWriter(fichier);
            try {
                writer.write("Nbannotateurs=" + nba + "\r\n");
                writer.write("Nbclasses="  + nbc + "\r\n");
                writer.write("Nbobservables="  + nbobs + "\r\n");
                for(int i=0; i<nba; i++) {
        			for(int j=0; j<nbobs; j++) {
        				if(tab[i][j] != ""){
            				writer.write("" + tab[i][j] + "\t");
        				}else{
            				writer.write(".\t");
        				}

        			}
        			writer.write("\r\n");
        		}
            } finally {
                // quoiqu'il arrive, on ferme le fichier
                writer.close();
            }
        } catch (Exception e) {
            System.out.println("Impossible de creer le fichier");
        }
	}

}
