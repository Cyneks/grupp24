1) För att kunna köra programmet måste du ha installerat c++, g++, SDL3 samt make.
Öppna projektet i valfri IDE eller kommandotolken, om du är i en IDE öppna en terminal och
navigera till roten av projektmappen. Beroende på vilket operativsystem som används kan du behöva
ändra filerna i vscode mappen och make filen. För att kunna köra programmet måste det först kompileras i 
terminalen med kommandot 'make'. Efter kompilering går programmet att köras med 
kommandot './build/debug/play'. Om det inte skulle fungera kan du behöva lägga till 
citattecken, använd istället kommandot '"./build/debug/play"'

2) När spelet startas kommer det att finnas två knappar i mitten av skärmen. Med muspekaren tryck
på EXIT för att stänga spelet (alternativt ESC) eller START för att köra spelet.
Din spelare kommer att finnas i mitten av fönstret när spelet startas. Rör på spelaren med
W A S D tangenterna. Om spelaren kolliderar med en vägg går det inte att fortsätta i den riktningen
men spelaren kan dock röra sig längs väggen.
Använd piltangenterna för att skjuta en kula i pilens riktning. Fienderna
kommer att laddas in kontinuerligt från sidorna av fönstret och röra sig mot spelarens position.
Om en fiende kolliderar med spelaren förlorar du och spelfiguren försvinner. När en kula träffar
en fiende försvinner de och spelaren får ett poäng som syns på poängräknaren i det vänstra hörnet.
När spelaren kommer upp i 10 poäng vinner de. Om en spelare skjuter på en vägg kommer väggen progressivt 
att gå sönder, om spelaren skjuter väggen fem gånger försvinner den. 
Om spelaren dör eller vinner kan de starta om spelet genom att trycka på R.

