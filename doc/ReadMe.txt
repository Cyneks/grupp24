Syftet med dokumentationen är att underlätta granskning och testning av ert programmeringsprojekt. 
Dokumentationen är en "ReadMe.txt"-fil som innehåller två delar, 1) en INSTRUKTION för hur 
programmeringsprojektet skall byggas och köras, vilka förutsättningar som måste vara uppfyllda, 
sökvägar till resursfiler (bildfiler/ljudfiler/typsnitt mm), samt 2) en TESTBESKRIVNING vad en 
spelare förväntas göra i spelet, hur figurernas rörelser kontrolleras, mm (bakgrunden till 2, är 
att det inte alltid är så självklart hur Spelet faktiskt relevant testkörs, t.ex. hur spelaren 
får poäng mm).

1) För att kunna köra programmet måste du ha installerat c++, g++, SDL3 samt make.
Öppna projektet i valfri IDE eller kommandotolken, om du är i en IDE öppna en terminal och
navigera till roten av projektmappen. För att kunna köra programmet måste det först kompileras i 
terminalen med kommandot 'make'.Efter kompilering går programmet att köras med 
kommandot './build/debug/play'. Om det inte skulle fungera kan du behöva lägga till 
citattecken, använd istället kommandot '"./build/debug/play"'

2) När spelet startas kommer din spelare att finnas i mitten av fönstret. Rör på spelaren med
W A S D tangenterna. Om spelaren kolliderar med en vägg går det inte att fortsätta i den riktningen
men spelaren kan dock röra sig längs väggen.
Använd piltangenterna för att skjuta en kula i pilens riktning. Fienderna
kommer att laddas in kontinuerligt från sidorna av fönstret och röra sig mot spelarens position.
Om en fiende kolliderar med spelaren förlorar du och spelfiguren försvinner. När en kula träffar
en fiende försvinner de och spelaren får ett poäng som syns på poängräknaren i det vänstra hörnet.
Om en spelare skjuter på en vägg kommer väggen progressivt att gå sönder, om spelaren skjuter väggen
fem gånger försvinner den. Tryck på ESC tangenten för att avsluta spelet.

