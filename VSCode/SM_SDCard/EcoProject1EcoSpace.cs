namespace SM_SDCard
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Linq;
    using Eco.Handles;
    using Eco.Linq;
    using Eco.Services;
    using Eco.UmlCodeAttributes;

    [EcoSpace]
    [UmlTaggedValue("Eco.InitializeNullableStringsToNull", "true")]
    [UmlTaggedValue("Eco.GenerateMultiplicityConstraints", "true")]
    public partial class EcoProject1EcoSpace : Eco.Handles.DefaultEcoSpace
    {
        #region Eco Managed code
        private static ITypeSystemService typeSystemProvider;
        #endregion Eco Managed code

        public EcoProject1EcoSpace() : base()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Persist all changes to the domain objects.
        /// </summary>
        /// <remarks>
        /// This function persists all changes to the eco space, including object creation,
        /// object manipulation, changed associations and object deletions. After invoking this method
        /// all undo information is removed.
        /// If the application does not have any persistence layer defined the operation does nothing.
        /// </remarks>
        public void UpdateDatabase()
        {
            if ((Persistence != null) && (DirtyList != null))
            {
                Persistence.UpdateDatabaseWithList(DirtyList.AllDirtyObjects());
            }
        }

        #region Eco Managed code
        public static new ITypeSystemService GetTypeSystemService()
        {
            if (typeSystemProvider == null)
            {
                lock (typeof(EcoProject1EcoSpace))
                {
                    if (typeSystemProvider == null)
                        typeSystemProvider = MakeTypeService(typeof(EcoProject1EcoSpace));
                }
            }

            return typeSystemProvider;
        }

        protected override ITypeSystemService GetTypeSystemProvider()
        {
            return EcoProject1EcoSpace.GetTypeSystemService();
        }
        #endregion

        // Add user written methods here
    }
}
