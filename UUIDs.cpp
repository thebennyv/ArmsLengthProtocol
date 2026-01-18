#include "UUIDs.h"

/*static*/ const BLEUUID UUIDs::UUID_SCRIPT_SERVICE   ("76584474-8f03-4154-8487-bbd3e0c89b13");
/*static*/ const BLEUUID UUIDs::UUID_HELMET_SERVICE   ("73a913b7-44e8-42e6-8359-9f0e8ae7533c");
/*static*/ const BLEUUID UUIDs::UUID_CHEST_SERVICE    ("947e5e78-e1d0-4ee2-b1a8-24c3381d1aac");
/*static*/ const BLEUUID UUIDs::UUID_BACK_SERVICE     ("3b45daef-48b3-4c02-b772-166d5805b8de");
/*static*/ const BLEUUID UUIDs::UUID_RIGHT_ARM_SERVICE("31894491-746b-466c-b500-99830cf8d628");
/*static*/ const BLEUUID UUIDs::UUID_LEFT_ARM_SERVICE ("22e9c99f-6c32-4446-975d-41aee40f13f9");
/*static*/ const BLEUUID UUIDs::UUID_RIGHT_LEG_SERVICE("4840719a-e6ce-4483-9eaf-8e40d4aba935");
/*static*/ const BLEUUID UUIDs::UUID_LEFT_LEG_SERVICE ("96afc7ab-4f5a-44ec-8de1-a3a049e4c660");

/*static*/ const BLEUUID UUIDs::UUID_DEFAULTS_SCRIPT_CHARACTERISTIC      ("116ddf94-21ff-4a96-ad82-390a67935a10");
/*static*/ const BLEUUID UUIDs::UUID_THREAT_DISPLAY_SCRIPT_CHARACTERISTIC("ef480ef5-1a60-4af9-8192-99e70d2ea9a4");
/*static*/ const BLEUUID UUIDs::UUID_ANGRY_SCRIPT_CHARACTERISTIC         ("68557009-952e-4677-a8a1-378370b678c1");
/*static*/ const BLEUUID UUIDs::UUID_READY_TO_FLY_SCRIPT_CHARACTERISTIC  ("329e2b4e-9e13-40c2-b190-7195667e3f98");
/*static*/ const BLEUUID UUIDs::UUID_FLAPS_TEST_SCRIPT_CHARACTERISTIC    ("46e82435-0674-453b-8f21-329812a4be65");

/*static*/ const BLEUUID UUIDs::UUID_EYES_CHARACTERISTIC     ("7d555942-4d5b-4de7-8f24-cae5c0853d6c");
/*static*/ const BLEUUID UUIDs::UUID_FACEPLATE_CHARACTERISTIC("3eba6a68-92d6-4a21-82c1-e12ce4365d9b");
/*static*/ const BLEUUID UUIDs::UUID_SPEECH_CHARACTERISTIC   ("650cf97a-ac8c-4362-8c27-62866d44ab90");

/*static*/ const BLEUUID UUIDs::UUID_ARC_REACTOR_CHARACTERISTIC("0ecf42f0-e25e-4402-b240-366ab1fc8988");
/*static*/ const BLEUUID UUIDs::UUID_CHEST_FLAPS_CHARACTERISTIC("3c652f8d-86cf-4b82-b282-54e01766b13e");

/*static*/ const BLEUUID UUIDs::UUID_BACK_FLAPS_CHARACTERISTIC("ce0be930-1c54-49fc-82f5-33976d4ff31a");

/*static*/ const BLEUUID UUIDs::UUID_RIGHT_ARM_LASER_CHARACTERISTIC   ("fd4bac79-5f87-4d82-b5e3-938f85fc2f79");
/*static*/ const BLEUUID UUIDs::UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC  ("d94a19ce-b78e-4fa4-82aa-186adba180ff");
/*static*/ const BLEUUID UUIDs::UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC("7cb6fcea-be0e-45d0-9588-95d439028093");

/*static*/ const BLEUUID UUIDs::UUID_LEFT_ARM_LASER_CHARACTERISTIC   ("b04e7bf4-97e9-44f4-8cf4-29ea8c18da6b");
/*static*/ const BLEUUID UUIDs::UUID_LEFT_ARM_ROCKET_CHARACTERISTIC  ("ffc1d60c-870c-48e6-ab73-319154f02830");
/*static*/ const BLEUUID UUIDs::UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC("7fcaac5d-7d04-48d6-bd2f-82bd5795b36e");

/*static*/ const BLEUUID UUIDs::UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC("80d0def8-dad8-41fa-83cc-6a87be1f55a2");

/*static*/ const BLEUUID UUIDs::UUID_LEFT_LEG_FLAPS_CHARACTERISTIC("f3316554-6bc5-44e9-9013-bb350a0debc3");

/*static*/ String UUIDs::nameOf(const BLEUUID &uuid)
{
    if (uuid == UUID_SCRIPT_SERVICE) return "Script Svc";
    if (uuid == UUID_HELMET_SERVICE) return "Helmet Svc";
    if (uuid == UUID_CHEST_SERVICE) return "Chest Svc";
    if (uuid == UUID_BACK_SERVICE) return "Back Svc";
    if (uuid == UUID_RIGHT_ARM_SERVICE) return "Right Arm Svc";
    if (uuid == UUID_LEFT_ARM_SERVICE) return "Left Arm Svc";
    if (uuid == UUID_RIGHT_LEG_SERVICE) return "Right Leg Svc";
    if (uuid == UUID_LEFT_LEG_SERVICE) return "Left Leg Svc";

    if (uuid == UUID_DEFAULTS_SCRIPT_CHARACTERISTIC) return "Defaults Script";
    if (uuid == UUID_THREAT_DISPLAY_SCRIPT_CHARACTERISTIC) return "Threat Display Script";
    if (uuid == UUID_ANGRY_SCRIPT_CHARACTERISTIC) return "Angry Script";
    if (uuid == UUID_READY_TO_FLY_SCRIPT_CHARACTERISTIC) return "Ready To Fly Script";
    if (uuid == UUID_FLAPS_TEST_SCRIPT_CHARACTERISTIC) return "Flaps Test Script";
    
    if (uuid == UUID_EYES_CHARACTERISTIC) return "Eyes";
    if (uuid == UUID_FACEPLATE_CHARACTERISTIC) return "Faceplate";
    if (uuid == UUID_SPEECH_CHARACTERISTIC) return "Speech";

    if (uuid == UUID_ARC_REACTOR_CHARACTERISTIC) return "Arc Reactor";
    if (uuid == UUID_CHEST_FLAPS_CHARACTERISTIC) return "Chest Flaps";

    if (uuid == UUID_BACK_FLAPS_CHARACTERISTIC) return "Back Flaps";

    if (uuid == UUID_RIGHT_ARM_LASER_CHARACTERISTIC) return "Right Arm Laser";
    if (uuid == UUID_RIGHT_ARM_ROCKET_CHARACTERISTIC) return "Right Arm Rocket";
    if (uuid == UUID_RIGHT_ARM_REPULSOR_CHARACTERISTIC) return "Right Arm Repulsor";

    if (uuid == UUID_LEFT_ARM_LASER_CHARACTERISTIC) return "Left Arm Laser";
    if (uuid == UUID_LEFT_ARM_ROCKET_CHARACTERISTIC) return "Left Arm Rocket";
    if (uuid == UUID_LEFT_ARM_REPULSOR_CHARACTERISTIC) return "Left Arm Repulsor";

    if (uuid == UUID_RIGHT_LEG_FLAPS_CHARACTERISTIC) return "Right Leg Flaps";

    if (uuid == UUID_LEFT_LEG_FLAPS_CHARACTERISTIC) return "Left Leg Flaps";

    return "UNKNOWN_UUID";
}

// https://www.UUIDgenerator.com/
// Spare UUIDs for future use:
// "48ed2108-0353-4273-ad66-8fe4b060fa6f"
// "b241f952-713e-42c5-81f9-89adb4722d99"
// "9d10d39b-0801-47b7-a58e-13412ca8a1cb"
// "9d725834-cb1e-483f-8027-5d73686079b5"
// "373b918c-0a61-415b-9309-d3103164ed76"
// "b338f2b7-7992-41a3-88d5-83aec904c5c9"
// "f5da82ef-a929-4600-a95a-8a424a1b50e2"
// "a8968249-e8c1-470f-8a9b-6ff947159294"
// "0d759703-6784-4bcc-9edb-86fe56a9ca3b"
// "cfde3226-6a40-4cba-b599-9c55d5e08ce0"
// "03d33578-c37a-48dd-8c9c-2150f0f24e7b"
// "7818d716-9cca-4abc-b231-9494a274930d"
// "cb32a903-16d2-49a2-9cec-b37dd2f73003"
// "aaa586ab-d79b-4f30-9c12-35777806aaaf"
// "dad7e4f6-bc4f-416a-9942-53d0e95fd081"
// "2f208c11-df74-4231-88ef-28d78466a336"
// "d082777c-09e9-460c-b27c-c4cdc626fda4"
// "45160352-86b2-4de2-a9a2-c972a46b7d7e"
// "c68e9825-98e6-46e5-bedf-209757f8478a"
// "79b2ccee-9c87-418a-b405-381141da026a"
// "d2de2920-01d9-4a9a-99b4-742239089359"
// "4cf23738-1e64-4d9f-95ee-48576845a4b2"
// "889f35b8-c1f3-4572-98e9-38ac5b191ff3"
// "488e6bc4-fbf9-4f36-892d-559a4d8666a7"
// "64eba5fd-21be-4970-9562-0a3da916f794"
// "5073fc5d-9743-4640-b650-e7670dd39b42"
// "9656ea09-609a-4faa-bbec-6f719927c3c7"
// "f3a7e0ad-1b55-4944-bf60-115775c5bb38"
// "9779e86c-695f-4fef-9311-adbb869e2a43"
// "6a780f06-ee06-46c5-b1ee-84965202572a"
// "71323570-4937-4fba-9250-e5cf48833310"
// "39f7147f-1372-446f-8682-f984e107be71"
// "4a2305ed-24c9-43fa-96ba-03fa8c8f9260"
// "d2b03878-7e27-4694-b555-d400ffad8c17"
// "2b061a1f-dddc-4999-92f7-6cb4697c5dfd"
// "565bf143-611f-40f4-b29d-741515f1d6ab"
// "4cc83bd6-7807-4ed3-b1f9-638737e782b2"
// "837fb1ff-eea0-4455-a8cb-27df5f95ad17"
// "b3e5a430-4f79-46dc-8042-5772476288b0"
// "b53e4d36-4437-4087-9bb1-65e0d45b1683"
// "51ce1e25-79fe-430c-b7f9-0e33a8546e3d"
// "4132fe37-ab5b-4cd0-beb2-6c4aa4d05735"
// "589e8f07-da8b-4a0c-9747-3329cc6e5606"
// "35fe8560-1763-44f8-8193-2bed5b54f583"
// "eacacf36-0c82-4c0e-bdda-ac3476a735ff"
// "fe34aa64-72ab-4535-9e14-fbe3f6f80bda"
// "7d537d7a-e359-40a0-8173-8e113414f430"
// "e9bcf24e-7bda-4c2c-a6c0-f2b4f4d7c2b0"
// "acc2ea6e-a4c7-409b-a5ca-8061e98da83c"
// "97470a3b-9a28-4ddb-a210-772762d86220"
// "6ee6ec0d-7029-4f49-a1ae-c813dbdc5b8f"
// "369acf0c-dea5-4f40-a809-0484533198d3"
// "f1807d05-4437-4aff-9ed7-bab1004d0e50"
// "76b649a2-94f4-42a7-a5cc-9ee3ec8bbad6"
// "bc7c1dfe-1bec-4613-be59-c349eba4a68a"
// "527744d7-b051-484f-a4ab-9a5060e886e9"
// "206df19d-6455-4dbf-b024-77c6bcc1959c"
// "05d9a26e-3c7e-492e-9b52-fda7a00a78de"
// "101cafc6-9bb5-4fa7-b113-e80ec982b3af"
// "a210364f-5027-41d3-ae8b-de1a3f492fcd"
// "04da23e7-18eb-4900-80ff-4cf83e595de1"
// "fba43b1d-513c-4eee-9dfe-2484b9bc6215"
// "491d8edb-c4b7-49c8-ae35-05a70a5123b5"
// "0b3bef95-7ea1-4fea-872d-e13a7e233df1"
// "3b00a5e9-8831-45f9-94cf-1039cffcb441"
// "b7f0b1eb-4e38-41aa-b4d8-073332cc2599"
// "13498132-8c75-4dc5-af35-f1a1ec45214a"
// "bb5ecef3-cf6e-4882-9d10-6daa126f7f8d"
// "1cb73ac9-2713-4820-925f-eb17439d5f3f"
// "212d32bf-5aaf-4dce-8983-06a2529e26cd"
// "458f09a3-858f-4ae5-a5c4-741bc9a802c4"
// "37f349f3-8ff0-4395-ac0d-c70c1584a102"
// "009a555e-e010-4c25-9475-1ff943137b01"
// "577b0eea-def7-4322-82e2-5c9b882fb601"
// "c0020c06-2cc5-41d0-bfc5-f773f7ca7f12"
// "32b19165-c4d6-43b8-a19d-08696931b95f"
// "81540660-d5f3-4965-9a2a-775477a1274e"
// "4b9f0ff2-5b6d-4860-b4bf-b8b34e21cdad"
// "dbf2e23d-9f44-4e93-846d-7eb159f56646"
// "6eed8565-947b-4412-a115-234b062a960a"
// "418a939a-cd20-4e63-8cd6-e55fb0508b26"
// "8fb77674-d580-4408-ad3a-e68f8cc2fd88"
// "e0ea01a1-d64d-4ef5-8c3b-b685944378c2"
