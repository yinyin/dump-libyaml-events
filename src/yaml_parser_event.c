#include <stdio.h>

#include <yaml.h>


static void print_event_name(const char * event_name) {
	printf("%s:\n", event_name);
}

static void print_scalar_content(yaml_event_t * event_p) {
	printf("  - value=[%s], length=%d\n", (event_p->data.scalar.value), (int)(event_p->data.scalar.length));
}

static void print_yaml_event(yaml_event_t *event_p) {
	yaml_event_type_t event_type;
	const char * event_name;
	event_type = event_p->type;
	event_name = "?";
	switch(event_type) {
	case YAML_NO_EVENT:
		print_event_name("YAML_NO_EVENT");
		break;
	case YAML_STREAM_START_EVENT:
		print_event_name("YAML_STREAM_START_EVENT");
		break;
	case YAML_STREAM_END_EVENT:
		print_event_name("YAML_STREAM_END_EVENT");
		break;
	case YAML_DOCUMENT_START_EVENT:
		print_event_name("YAML_DOCUMENT_START_EVENT");
		break;
	case YAML_DOCUMENT_END_EVENT:
		print_event_name("YAML_DOCUMENT_END_EVENT");
		break;
	case YAML_ALIAS_EVENT:
		print_event_name("YAML_ALIAS_EVENT");
		break;
	case YAML_SCALAR_EVENT:
		print_event_name("YAML_SCALAR_EVENT");
		print_scalar_content(event_p);
		break;
	case YAML_SEQUENCE_START_EVENT:
		print_event_name("YAML_SEQUENCE_START_EVENT");
		break;
	case YAML_SEQUENCE_END_EVENT:
		print_event_name("YAML_SEQUENCE_END_EVENT");
		break;
	case YAML_MAPPING_START_EVENT:
		print_event_name("YAML_MAPPING_START_EVENT");
		break;
	case YAML_MAPPING_END_EVENT:
		print_event_name("YAML_MAPPING_END_EVENT");
		break;
	default:
		print_event_name("???");
	}
}

static int parse(const char * file_path) {
	FILE * fp;
	int completed;
	completed = 0;
	if (NULL == (fp = fopen(file_path, "r"))) {
		perror("cannot open file");
		return -1;
	}
	do {
		yaml_parser_t parser;
		yaml_event_t event;
		yaml_parser_initialize(&parser);
		yaml_parser_set_input_file(&parser, fp);
		do {
			if (!yaml_parser_parse(&parser, &event)) {
				break;
			}
			print_yaml_event(&event);
			completed = (YAML_STREAM_END_EVENT == event.type);
			yaml_event_delete(&event);
		} while(0 == completed);
		yaml_parser_delete(&parser);
	} while(0);
	fclose(fp);
	if (0 == completed) {
		return -2;
	}
	return 0;
}

int main(int argc, char ** argv) {
	int retcode;
	retcode = parse(argv[1]);
	fprintf(stderr, "Parsing result code: %d\n", retcode);
	return 0;
}
