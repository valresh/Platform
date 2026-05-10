MODEL("Насосы", true)
	DEFECT(defContamination, "Загазованность")
		DEF_D( _defContamination, "Загазованость в %ПДК", 60., 0., 200. )
	DEFECT(defReset, "Сброс насоса")