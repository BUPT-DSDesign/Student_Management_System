import { defineStore } from 'pinia'
import { CourseTable, AddCourse, DeleteCourse, EditCourse, inquiryCourse } from '@/api/course'
import { ref } from 'vue'

export const useCourseStore = defineStore('course', () => {
    const courseList = ref([])
    const searchCourseList = ref([])


    const GetCourseTable = async () => {
        try {
            const res = await CourseTable()

            if (res.data.status_code == 0) {
                courseList.value = res.data.course_list
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }

    const AddCourseInfo = async (data) => {
        try {
            const res = await AddCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }
    const DeleteCourseInfo = async (data) => {
        try {
            const res = await DeleteCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }
    const EditCourseInfo = async (data) => {
        try {
            const res = await EditCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }
    const inquiryCourseInfo = async (data) => {
        try {
            const res = await inquiryCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                searchCourseList.value = res.data.course_list
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        courseList,
        searchCourseList,
        GetCourseTable,
        AddCourseInfo,
        DeleteCourseInfo,
        EditCourseInfo,
        inquiryCourseInfo
    }
})
