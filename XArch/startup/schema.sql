create table if not exists 
		   Patients(id integer primary key autoincrement, 
		   name text, 
		   birthday text, 
		   sex int)

create table if not exists 
		   Modalities(id integer primary key autoincrement, 
		   name text, 
		   template text)

create table if not exists 
		   Devices(id integer primary key autoincrement, 
		   name text, 
		   modality integer references Modalities(id))

create table if not exists 
		   Regions(id integer primary key autoincrement, 
		   name text, 
		   modality integer references Modalities(id))

create table if not exists 
		   Contrasts(id integer primary key autoincrement, 
		   name text, 
		   modality integer references Modalities(id))

create table if not exists 
		   OrderedBy(id integer primary key autoincrement, 
		   name text)

create table if not exists 
		   Examinations(id integer primary key autoincrement, 
		   patient integer references Patients(id), 
		   dt text, 
		   modality integer references Modalities(id), 
		   device integer references Devices(id), 
		   region integer references Regions(id), 
		   contrast integer references Contrasts(id), 
		   contrastAmount real, 
		   orderedBy integer references OrderedBy(id), 
		   content text, 
		   conclusion text)

create table if not exists 
		   TemplateItems(id integer primary key autoincrement, 
		   regionId integer references Regions(id), 
		   idx integer, 
		   value text)

create table if not exists 
		   CommonPhrases(id integer primary key autoincrement, 
		   regionId integer references Regions(id), 
		   value text)

create table if not exists 
		   AltTemplateItems(id integer primary key autoincrement, 
		   parentId integer references TemplateItems(id), 
		   value text)

create table if not exists 
		   ConclusionTemplateItems(id integer primary key autoincrement, 
		   regionId integer references Regions(id), 
		   value text)

create table if not exists 
		   ConclusionAltTemplateItems(id integer primary key autoincrement, 
		   parentId integer references ConclusionTemplateItems(id), 
		   value text)
